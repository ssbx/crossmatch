/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * SimpleActiveList implements an ActiveList as a simple
 * double ended queue of active objects.
 *
 * This is not an efficient implementation and should
 * not be used for large active lists. This is provided
 * as a comparison to the indexed active list and to
 * ensure the results obtained by that structure are
 * valid.
 */

#ifndef SIMPLEACTIVELIST_DEFINED
#define SIMPLEACTIVELIST_DEFINED

#include <deque>

#include "ActiveList.h"

class ActiveObject;
class Object;
class ObjectPairConsumer;
class ObjectConsumer;


class SimpleActiveList : public ActiveList
{
  public:
    SimpleActiveList();
    virtual ~SimpleActiveList();

    virtual void deletePriorObjects(double boundary,
                                    ObjectConsumer * uActiveCons);
    virtual void pushBack(Object const * object,
                          bool matchedPreviously=false);
    virtual bool testObject(Object const * object,
                            double upperLimitOnDistance,
                            ObjectPairConsumer * matchedConsumer);
    virtual void clear(ObjectConsumer * uActiveCons);
    virtual void finished(ObjectConsumer * uActiveCons);
    virtual bool isEmpty();
    virtual ActiveObject * popFront();

  private:
    typedef std::deque<ActiveObject *> SAL;
    SAL    activeStructure;
    double activeMaxSD;

    SimpleActiveList(SimpleActiveList const &);
    SimpleActiveList & operator=(SimpleActiveList const &);
};

#endif // ifndef SIMPLEACTIVELIST_DEFINED
