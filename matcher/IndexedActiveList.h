/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * IndexedActiveList implements an ActiveList with a data
 * structure that is very efficient for:
 *  - adding objects in order of declination;
 *  - removing all objects up to a given declination limit; and
 *  - reporting all objects that fall within a given right
 *    ascension range.
 */

#ifndef INDEXEDACTIVELIST_DEFINED
#define INDEXEDACTIVELIST_DEFINED

#include "ActiveList.h"
#include "ALStructure.h"

class ActiveObject;
class Object;
class ObjectPairConsumer;
class ObjectConsumer;


class IndexedActiveList : public ActiveList
{
  public:
    IndexedActiveList();
    virtual ~IndexedActiveList();

    virtual void deletePriorObjects(double boundary,
                                    ObjectConsumer * uActiveCons);
    virtual void pushBack(Object const * object,
                          bool matchedPreviously=false);
    virtual bool testObject(Object const * testObject,
                            double upperLimitOnDistance,
                            ObjectPairConsumer * matchedConsumer);
    virtual void clear(ObjectConsumer * uActiveCons);
    virtual void finished(ObjectConsumer * uActiveCons);
    virtual bool isEmpty();
    virtual ActiveObject * popFront();

  private:
    ALStructure * activeStructure;
    double        activeMaxSD;

    IndexedActiveList(IndexedActiveList const &);
    IndexedActiveList & operator=(IndexedActiveList const &);

    ALElement * remove(ALElement * i);
};

#endif // ifndef INDEXEDACTIVELIST_DEFINED

