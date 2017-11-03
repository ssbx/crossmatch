/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * This is the program used to perform the nearest neighbour benchmarking.
 *
 * Command line options are:
 *
 * <a_dir|0>        The directory path containing the input files, 0 for a single file.
 * <a_file>         The common file name prefix, or full filename (including path).
 * <binary|ascii>   indicates if the file is binary or ascii.
 * [merge]          currently only required for 2MASS.
 * <maxDistInArcSecs> as it says.
 * <index|list>     Indexed or list for the active list.
 * <filter|nested|sweep>
 *                  Use CM filter, or generic nested loop, or generic plane sweep.
 */

#include <iostream>
#include <string.h>
#include "String.h"

#include "ActiveList.h"
#include "DecPlaneSweepFilter.h"
#include "FileProducerFactory.h"
#include "FilePairWriter.h"
#include "FileWriter.h"
#include "Filter.h"
#include "IndexedActiveList.h"
#include "Matcher.h"
#include "Neighbours.h"
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
          << " <dir|0> <file> <binary|ascii> [merge]"
          << " <maxDistInArcSecs> <index|list> <filter|nested|sweep>" << std::endl;
}

int main(int argc, char * argv[])
{
#ifdef PLOT_TIMES
  Timer::globalTimer()->cont();        // need to start the timer
#endif

  int status = 0;

  if (argc != 7 && argc != 8)
  {
    usage(argv[0]);
    status = 1;
  }
  else
  {
    int param = 1;
    char * dir = argv[param++];
    char * name = argv[param++];
    bool binary = (strcmp(argv[param++], "binary") == 0);
    bool merge = false;
    if (strcmp(argv[param], "merge") == 0)
    {
      merge = true;
      param++;
    }

    double maxDist = atof(argv[param++])/3600.0;
    bool useIndex = (strcmp(argv[param++], "index") == 0);
    bool useFilter =     (strcmp(argv[param], "filter") == 0);
    bool useNestedLoop = (!useFilter && strcmp(argv[param++], "nested") == 0);

    if (useFilter)
      std::cout << "Using NN filter           : true" << std::endl; 
    else if (useNestedLoop)
      std::cout << "Using generic nested loop : true" << std::endl;
    else
      std::cout << "Using generic plane sweep : true" << std::endl;
    std::cout << "Using index active list   : " << ((useIndex) ? "true" : "false") << std::endl;
    std::cout << "Max distance (Dec Degs)   : " << maxDist << std::endl;

    // example to output the neighbours to a file
//    ObjectPairConsumer accWithinDistPairCons(WithinDistanceRefine::name() + " accept",
//                                            new FilePairWriter(new TextFileDatumWriter(), "ACC"));
    ObjectPairConsumer accWithinDistPairCons(WithinDistanceRefine::name() + " accept");
    ObjectPairConsumer rejWithinDistPairCons(WithinDistanceRefine::name() + " reject");

    WithinDistanceRefine withinDistRefine(maxDist,
                                          &accWithinDistPairCons,
                                          &rejWithinDistPairCons);

    ObjectPairRefineConsumer pairCons(((useNestedLoop) ? NestedLoopFilter::name()
                                                       : DecPlaneSweepFilter::name()),
                                      &withinDistRefine);

    ObjectProducer * prod = FileProducerFactory::instance()->
                                createProducer(dir, name, binary, merge, false);

    // example to output the non matches to a file
//    ObjectConsumer cons("points reject", new FileWriter(new TextFileDatumWriter(), "NM"));
    ObjectConsumer cons("points reject");

    ActiveList * aList = (useIndex) ? (ActiveList *) new IndexedActiveList()
                                    : (ActiveList *) new SimpleActiveList();
    Matcher * matcher = new Neighbours(prod, aList, &pairCons, &cons, maxDist);
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
