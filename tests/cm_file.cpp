/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * This is the program used to perform the cross match benchmarking.
 *
 * At first it may seem an overwhelming number of parameters, but there
 * aren't really that many:
 *
 * The first 5 describe the input file(s):
 *
 * <a_dir|0>        The directory path containing the input files, 0 for a single file.
 * <a_file>         The common file name prefix, or full filename (including path).
 * <binary|ascii>   indicates if the file is binary or ascii.
 * [merge]          currently only required for 2MASS.
 * <a_maxSD>        the max SD value for the input data.
 *
 * The next 4 (or 5 if "merge" used) are for the other catalogue.
 *
 * <z_alpha>        We have used a value of 1.96 for our tests.
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
#include "AngularSeparationRefine.h"
#include "BoundingBoxRefine.h"
#include "CrossMatch.h"
#include "DecPlaneSweepFilter.h"
#include "FileProducerFactory.h"
#include "FileWriter.h"
#include "FilePairWriter.h"
#include "Filter.h"
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

static void usage(char const * const prog_name)
{
    std::cerr << prog_name
        << " <a_dir|0> <a_file> <binary|ascii> [merge] <a_maxSD>"
        << " <b_dir|0> <b_file> <binary|ascii> [merge] <b_maxSD>"
        << " <z_alpha> <index|list> <bb|as> <filter|nested|sweep>" << std::endl;
}

int main(int argc, char * argv[])
{
#ifdef PLOT_TIMES
    Timer::globalTimer()->cont();        // need to start the timer
#endif

    if (argc < 13 || argc > 15) {
        usage(argv[0]);
        return 1;
    }

    int param = 1;
    char * aDir = argv[param++];
    char * aName = argv[param++];
    bool aBinary = (strcmp(argv[param++], "binary") == 0);
    bool aMerge = false;
    if (strcmp(argv[param], "merge") == 0)
    {
        aMerge = true;
        param++;
    }
    double aMaxSD = atof(argv[param++]);

    char * bDir = argv[param++];
    char * bName = argv[param++];
    bool bBinary = (strcmp(argv[param++], "binary") == 0);
    bool bMerge = false;
    if (strcmp(argv[param], "merge") == 0)
    {
        bMerge = true;
        param++;
    }
    double bMaxSD = atof(argv[param++]);

    double z_alpha = atof(argv[param++]);
    Object::zAlpha = z_alpha;

    bool useIndex =      (strcmp(argv[param++], "index") == 0);
    bool useBB =         (strcmp(argv[param++], "bb") == 0);
    bool useFilter =     (strcmp(argv[param], "filter") == 0);
    bool useNestedLoop = (!useFilter && strcmp(argv[param++], "nested") == 0);

    if (useFilter)
        std::cout << "Using CM filter           : true" << std::endl; 
    else if (useNestedLoop)
        std::cout << "Using generic nested loop : true" << std::endl;
    else
        std::cout << "Using generic plane sweep : true" << std::endl;
    std::cout << "Using index active list   : " << ((useIndex) ? "true" : "false") << std::endl;
    std::cout << "Using full BB refine      : " << ((useBB) ? "true" : "false") << std::endl;

    /* example to consume successful angular separation refine candidates to a file */
    /*
       ObjectPairConsumer accAngSepPairCons(AngularSeparationRefine::name() + " accept");
       new FilePairWriter(new TextFileDatumWriter(), "ASacc"));
       */
    ObjectPairConsumer accAngSepPairCons(AngularSeparationRefine::name() + " accept");
    ObjectPairConsumer rejAngSepPairCons(AngularSeparationRefine::name() + " reject");

    AngularSeparationRefine angSepRefine(&accAngSepPairCons,
            &rejAngSepPairCons);

    ObjectPairRefineConsumer angSepConsumer(BoundingBoxRefine::name(),
            &angSepRefine);

    ObjectPairConsumer rejBBPairCons(BoundingBoxRefine::name() + " reject");

    BoundingBoxRefine bbRefine(&angSepConsumer, &rejBBPairCons);

    // depending on value of useBB, set the CrossMatchRefine
    ObjectPairRefineConsumer pairCons(((useNestedLoop) ? NestedLoopFilter::name()
                : DecPlaneSweepFilter::name()),
            ((useBB) ? (Refine*) &bbRefine
             : (Refine*) &angSepRefine));

    /* example to consume non matches to a file */
    /*
       ObjectConsumer uaCons("A points reject",
            new FileWriter(new TextFileDatumWriter(), "NMA"));
       ObjectConsumer ubCons("B points reject",
            new FileWriter(new TextFileDatumWriter(), "NMB"));
       */
    ObjectConsumer uaCons("A points reject");
    ObjectConsumer ubCons("B points reject");

    ObjectProducer *aProd = FileProducerFactory::instance()->createProducer(
                                aDir, aName, aBinary, aMerge, false, aMaxSD);
    ObjectProducer *bProd = FileProducerFactory::instance()->createProducer(
                                bDir, bName, bBinary, bMerge, false, bMaxSD);


    ActiveList  *aList   = (useIndex) ? (ActiveList *) new IndexedActiveList() : (ActiveList *) new SimpleActiveList();
    Matcher     *matcher = new CrossMatch(aProd, bProd, aList , &pairCons, &uaCons, &ubCons);
    Filter      *filter  = 0;

    if (useFilter)
        filter = new Filter(matcher);
    else if (useNestedLoop)
        filter = new NestedLoopFilter(matcher);
    else
        filter = new DecPlaneSweepFilter(matcher);

    filter->filter();

    delete filter;

    return 0;
}
