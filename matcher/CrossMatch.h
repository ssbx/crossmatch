/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Implements the Matcher interface for catalogue cross matching.
 *
 * Implements the generic matcher methods required for various filter
 * algorithms. Also provides an efficient plane sweep filter.
 *
 * Two object producers are required, plus two object consumers to report
 * those objects that don't match and a pair consumer for those that do.
 *
 * The active list is required to maintain an efficient set of active
 * objects for testing against the test obejcts.
 *
 * The producers and active list are owned by this class and are deleted
 * by the destructor.
 */

#ifndef CROSSMATCH_DEFINED
#define CROSSMATCH_DEFINED

#include "String.h"

#include "ActiveList.h"
#include "Matcher.h"
#include "ObjectConsumer.h"

class Object;
class ObjectPairConsumer;
class ObjectProducer;


class CrossMatch : public Matcher
{
  public:
    CrossMatch(ObjectProducer * activeProd,
               ObjectProducer * testProd,
               ActiveList * activeList,
               ObjectPairConsumer * pairCons,
               ObjectConsumer * uActiveCons,
               ObjectConsumer * uTestCons);
    virtual ~CrossMatch();

    virtual String const & getName() const { return name(); };
    static String const & name() { return s_name; };

    virtual void getReady();
    virtual void finished();
    virtual void doFilter();

    virtual Object const * nextActiveObject() { return nextObject(activeProducer); };
    virtual Object const * nextTestObject() { return nextObject(testProducer); };

    virtual double getLowerBound(Object const * testObject);
    virtual double getUpperBound(Object const * testObject);

    virtual void addActiveObject(Object const * activeObject)
                    { activeList->pushBack(activeObject); };

    virtual void reportActiveNoMatch(Object const * activeObject)
                    { reportNoMatch(uActiveConsumer, activeObject); };
    virtual void reportTestNoMatch(Object const * testObject)
                    { reportNoMatch(uTestConsumer, testObject); };

    virtual void test(Object const * testObject);

    virtual void flushActiveObjects(double lowerBound);
    virtual void flushActiveObjects(Object const * activeObject);

  protected:
    virtual double setAngularUpperLimit(Object const * testObject) const;

  private:
    static String const s_name;

    ObjectProducer * activeProducer;
    ObjectProducer * testProducer;
    ObjectConsumer * uActiveConsumer;
    ObjectConsumer * uTestConsumer;
    ActiveList *     activeList;

    CrossMatch();
    CrossMatch(CrossMatch const &);
    CrossMatch & operator=(CrossMatch const &);

    void test(Object const * & testObject, double & lowerBound, double & upperBound);

    void raceThroughProducer(ObjectProducer * producer,
                             Object const * currentObject,
                             ObjectConsumer * uCons);
    Object const * raceThroughProducer(ObjectProducer * producer,
                                       Object const * currentObject,
                                       double limit,
                                       ObjectConsumer * uCons);

    Object const * reportUnmatchedObject(ObjectProducer * producer,
                                         Object const * currentObject,
                                         ObjectConsumer * uCons);
};


#endif // ifndef CROSSMATCH_DEFINED
