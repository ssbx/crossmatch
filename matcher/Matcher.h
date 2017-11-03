/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Matcher is the interface that every matching implementation must instantiate.
 *
 * A matcher assumes there are two object producers: one for active
 * objects placed onto the active list, and one for test object, which are
 * tested against the active objects in the active list.
 *
 * Matches of active objects are reported to the pair consumer while
 * those objects not involved in a match are reported appropriatly
 * via the reportActiveNoMatch and reportTestNoMatch methods.
 *
 * A matcher may implement its own specialised filter implementation:
 * the doFilter method. The filter class will call the getReady and 
 * finished methods prior to and after filtering respectively.
 *
 * The getLowerBound and getUpperBound methods are the limits for the
 * active objects being in the active list, and are calculated in terms
 * of the current test object.
 *
 * The test method with objects is a "short cut" to testing if two 
 * candidate objects meet the matching criteria. This is not normally
 * used (except by the NestedLoopFilter class). Instead, should use the
 * test method providing a single testObject to test against all
 * objects in the active list.
 *
 * The final two flush methods are for cleaning up when one or the other
 * producers are finished.
 *
 * These classes should be understood in conjunction with the Filter
 * classes. The aim has been to abstract the filtering from the
 * matching so that different matching algorithms may be used with
 * various filters in combination without exploding the number of
 * class required. The success of this approach will be measured as
 * more filter or matching classes are implemented.
 */

#ifndef MATCHER_DEFINED
#define MATCHER_DEFINED

#include "String.h"

#include "ObjectConsumer.h"

class Object;
class ObjectProducer;
class ObjectPairConsumer;
class Profiler;


class Matcher
{
  public:
    virtual ~Matcher();

    virtual String const & getName() const=0;

    bool hasFilter() const { return hasOwnFilter; };

    virtual void getReady();
    virtual void finished();
    virtual void doFilter() {};

    virtual Object const * nextActiveObject()=0;
    virtual Object const * nextTestObject()=0;

    virtual double getLowerBound(Object const * testObject)=0;
    virtual double getUpperBound(Object const * testObject)=0;

    virtual void addActiveObject(Object const * activeObject)=0;

    virtual bool test(Object const * activeObject,
                      Object const * testObject);    
    virtual void test(Object const * testObject)=0;

    virtual void reportActiveNoMatch(Object const * activeObject)=0;
    virtual void reportTestNoMatch(Object const * testObject)=0;

    virtual void flushActiveObjects(double lowerBound)=0;
    virtual void flushActiveObjects(Object const * activeObject)=0;

  protected:
    ObjectPairConsumer * pairCons;
    Profiler *           profiler;
    
    Matcher(ObjectPairConsumer * pairCons, bool hasFilter=false);

    Object const * nextObject(ObjectProducer * producer);
    void reportNoMatch(ObjectConsumer * cons, Object const * obj)
                        { cons->report(obj); };

  private:
    bool hasOwnFilter;
    unsigned long long numObjects;

    Matcher();
    Matcher(Matcher const &);
    Matcher & operator=(Matcher const &);

    void incObjectCount();
};


#endif // ifndef MATCHER_DEFINED
