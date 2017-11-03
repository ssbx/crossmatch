/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * This is the program used to perform fixed radius cross match benchmarking.
 *
 * Note - this file is an alteration of cm_file.cpp, without the threshold
 * refine option and using the WithinDistanceRefine in place of AngularSeparationRefine.
 *
 * At first it may seem an overwhelming number of parameters, but there
 * aren't really that many:
 *
 * The first 4 describe the input file(s):
 *
 * <a_dir|0>        The directory path containing the input files, 0 for a single file.
 * <a_file>         The common file name prefix, or full filename (including path).
 * <binary|ascii>   Indicates if the file is in binary or ascii format
 * [merge]          currently only required for 2MASS.
 *
 * The next 3 (or 4 if "merge" used) are for the other catalogue.
 *
 * <maxDistInArcSecs> Match fixed radius, in arc seconds.
 * <index|list>     Indexed or list for the active list.
 * <bb|as>          Bounding Box test with Angular Separation, or just Angular Separation 
 * <filter|nested|sweep>
 *                  Use CM filter, or generic nested loop, or generic plane sweep.
 *
 * The output currently reports the number of matches (and non-matches) found.
 *
 * To write the output to a file, will need to modify the ObjectConsumers
 * (uaCons and ubCons) for the unmatched objects, or the various
 * ObjectPairConsumers used by the ObjectPairRefineConsumers.
 * Examples are included in comments below.
 */

#include <iostream>
#include <string.h>
#include "String.h"

#include "ActiveList.h"
#include "BoundingBoxRefine.h"
#include "DecPlaneSweepFilter.h"
#include "FileProducerFactory.h"
#include "FileWriter.h"
#include "FilePairWriter.h"
#include "Filter.h"
#include "FixedRadiusCrossMatch.h"
#include "IndexedActiveList.h"
#include "Matcher.h"
#include "NestedLoopFilter.h"
#include "Object.h"
#include "ObjectConsumer.h"
#include "ObjectPairConsumer.h"
#include "ObjectPairRefineConsumer.h"
#include "ObjectProducer.h"
#include "Refine.h"
#include "SimpleActiveList.h"
#include "TextFileDatumWriter.h"
#include "Timer.h"
#include "WithinDistanceRefine.h"


static void usage(char const * const prog_name)
{
  std::cerr << prog_name
            << " <a_dir|0> <a_file> <binary|ascii> [merge]"
            << " <b_dir|0> <b_file> <binary|ascii> [merge]"
            << " <maxDistInArcSecs> <index|list> <bb|as> <filter|nested|sweep>"
            << std::endl;
}

int main(int argc, char * argv[])
{
#ifdef PLOT_TIMES
  Timer::globalTimer()->cont();        // need to start the timer
#endif

  int status = 0;

  if (argc < 11 || argc > 13)
  {
    usage(argv[0]);
    status = 1;
  }
  else
  {
    int param = 1;
    char * aDir = argv[param++];
    char * aName = argv[param++];
    bool aBinary = true;
    if (strcmp(argv[param++], "ascii") == 0)
      aBinary = false;
    bool aMerge = false;
    if (strcmp(argv[param], "merge") == 0)
    {
      aMerge = true;
      param++;
    }

    char * bDir = argv[param++];
    char * bName = argv[param++];
    bool bBinary = true;
    if (strcmp(argv[param++], "ascii") == 0)
      bBinary = false;
    bool bMerge = false;
    if (strcmp(argv[param], "merge") == 0)
    {
      bMerge = true;
      param++;
    }

    double maxDist = atof(argv[param++])/3600.0;

    bool useIndex =      (strcmp(argv[param++], "index") == 0);
    bool useBB =         (strcmp(argv[param++], "bb") == 0);
    bool useFilter =     (strcmp(argv[param], "filter") == 0);
    bool useNestedLoop = (!useFilter && strcmp(argv[param], "nested") == 0);

    if (useFilter)
      std::cout << "Using FRCM filter         : true" << std::endl; 
    else if (useNestedLoop)
      std::cout << "Using generic nested loop : true" << std::endl;
    else
      std::cout << "Using generic plane sweep : true" << std::endl;
    std::cout << "Using index active list   : " << ((useIndex) ? "true" : "false") << std::endl;
    std::cout << "Using full BB refine      : " << ((useBB) ? "true" : "false") << std::endl;
    std::cout << "Max distance (Dec Degs)   : " << maxDist << std::endl;

    /* Consume successful angular separation refine candidates to a file */

/*
    ObjectPairConsumer accWithinDistPairCons(WithinDistanceRefine::name() + " accept",
                         new FilePairWriter(new TextFileDatumWriter(), "ASacc"));
*/
    ObjectPairConsumer accWithinDistPairCons(WithinDistanceRefine::name() + " accept");
    ObjectPairConsumer rejWithinDistPairCons(WithinDistanceRefine::name() + " reject");

    WithinDistanceRefine distanceRefine(maxDist, &accWithinDistPairCons,
                                                 &rejWithinDistPairCons);

    ObjectPairRefineConsumer bbCons(BoundingBoxRefine::name(), &distanceRefine);

    ObjectPairConsumer rejBBPairCons(BoundingBoxRefine::name() + " reject");

    BoundingBoxRefine bbRefine(&bbCons, &rejBBPairCons);

    // depending on value of useBB and useThreshold, set the appropriate refine
    ObjectPairRefineConsumer pairCons(useNestedLoop ? NestedLoopFilter::name()
                                                    : DecPlaneSweepFilter::name(),
                                      useBB ? (Refine*) &bbRefine
                                            : (Refine*) &distanceRefine);


    ObjectConsumer uaCons("A points reject");
    ObjectConsumer ubCons("B points reject");

    ObjectProducer * aProd = FileProducerFactory::instance()->createProducer(
                                        aDir, aName, aBinary, aMerge, true, false);
    ObjectProducer * bProd = FileProducerFactory::instance()->createProducer(
                                        bDir, bName, bBinary, bMerge, true, false);

    ActiveList * aList = (useIndex) ? (ActiveList *) new IndexedActiveList()
                                    : (ActiveList *) new SimpleActiveList();
    Matcher * matcher = new FixedRadiusCrossMatch(aProd, bProd, aList , &pairCons, &uaCons, &ubCons, maxDist);
    Filter * filter = 0;

    if (useFilter)
      filter = new Filter(matcher);
    else if (useNestedLoop)
      filter = new NestedLoopFilter(matcher);
    else
      filter = new DecPlaneSweepFilter(matcher);

    filter->filter();

    delete filter;
  }

  return status;
}
