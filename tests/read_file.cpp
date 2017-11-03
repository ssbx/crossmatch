/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * This is a simple test program that reads a catalogue file(s),
 * checks that it os sorted by declination and calculates the
 * the max standard deviations found.
 */

#include <stdio.h>
#include <iostream>
#include <string.h>

#include "Object.h"
#include "ObjectProducer.h"
#include "FileProducerFactory.h"

#ifdef TIMER
#include "Timer.h"
#endif

int main(int argc, char * argv[])
{
  if (argc != 4 && argc != 5 && argc != 6)
  {
    std::cerr << argv[0] << " <dir|0> <prefix|name> [large] <binary|ascii> [merge]" << std::endl;
  }
  else
  {
#ifdef TIMER
    Timer::globalTimer()->cont();        // need to start the timer
#endif

    int param = 1;
    char * dir = argv[param++];
    char * name = argv[param++];
    bool large = false;
    if (strcmp(argv[param], "large") == 0)
    {
      large = true;
      param++;
    }
    bool binary = (strcmp(argv[param++], "binary") == 0);
    bool merge = (argc == param+1 && strcmp(argv[param], "merge") == 0);

    ObjectProducer * prod = FileProducerFactory::instance()->
                                createProducer(dir, name, binary, merge, large);

    bool orderOk = true;
    double maxSD = 0.0;
    double currentDec = -90.0;
    unsigned long long cnt = 0;
    while (prod->hasNext())
    {
      Object * obj = prod->next();
      if (obj->getDec() < currentDec)
        orderOk = false;
      currentDec = obj->getDec();
      if (obj->getSD() > maxSD)
        maxSD = obj->getSD();
      delete obj;
      cnt++;
    }

    std::cout << "Read         : " << cnt << " records." << std::endl;
    std::cout << "Dec order is : " << ((orderOk) ? "ok" : "not ok!") << std::endl;
    std::cout << "Max SD       : " << maxSD << std::endl;

    delete prod;

#ifdef TIMER
    Timer::globalTimer()->reportTimes();
#endif
  }

  return 0;
}
