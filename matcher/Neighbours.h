/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Implements the Matcher interface for determining nearest neighbours
 * within a single catalogue.
 *
 * Implements the generic matcher methods required for various filter
 * algorithms. Also provides an efficient plane sweep filter.
 *
 * one object producer is required, plus an object consumer to report
 * those objects that don't match and a pair consumer for those that do.
 *
 * The active list is required to maintain an efficient set of active
 * objects for testing against the test obejcts.
 *
 * The producer and active list are owned by this class and are deleted
 * by the destructor.
 */

#ifndef NEIGHBOURS_DEFINED
#define NEIGHBOURS_DEFINED

#include "String.h"

#include "ActiveList.h"
#include "Matcher.h"

class Object;
class ObjectConsumer;
class ObjectPairConsumer;
class ObjectProducer;


class Neighbours : public Matcher
{
  public:
    Neighbours(ObjectProducer * prod,
               ActiveList * activeList,
               ObjectPairConsumer * pairCons,
               ObjectConsumer * uCons,
               double maxD);
    virtual ~Neighbours();

    virtual String const & getName() const { return name(); };
    static String const & name() { return s_name; };

    virtual void getReady();
    virtual void finished();
    virtual void doFilter();

    virtual Object const * nextActiveObject();
    virtual Object const * nextTestObject();

    virtual double getLowerBound(Object const * testObject);
    virtual double getUpperBound(Object const * testObject);

    virtual void addActiveObject(Object const * activeObject);

    virtual void reportActiveNoMatch(Object const * activeObject);
    virtual void reportTestNoMatch(Object const * testObject);

    virtual bool test(Object const * activeObject,
                      Object const * testObject);
    virtual void test(Object const * testObject);

    virtual void flushActiveObjects(double lowerBound);
    virtual void flushActiveObjects(Object const * activeObject);

  private:
    static String const s_name;

    ObjectProducer * producer;
    ObjectConsumer * uConsumer;
    ActiveList *     activeList;
    ActiveObject *   testActive;
    Object const *   testObj;
    bool             testMatchedPreviously;
    Object const *   currentObj;
    double           maxDistance;
    int              cnt;

    Neighbours();
    Neighbours(Neighbours const &);
    Neighbours & operator=(Neighbours const &);
};


#endif // ifndef NEIGHBOURS_DEFINED
