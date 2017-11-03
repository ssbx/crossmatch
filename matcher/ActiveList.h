/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * ActiveList defines the interface that any active list
 * data structure must implement.  It also provides some
 * simple methods that every concrete implementation will
 * make use of.
 *
 * Objects are pushed onto the back of the list and accessed
 * from the front.
 *
 * The testObject method searches through the active list for
 * active objects that match the provided testObject. Matched
 * pairs are reported to the matchedConsumer. Returns true
 * if a match was found.
 */

#ifndef ACTIVELIST_DEFINED
#define ACTIVELIST_DEFINED

class ActiveObject;
class Object;
class ObjectConsumer;
class ObjectPairConsumer;
class Profiler;


class ActiveList
{
  public:
    virtual ~ActiveList();
    virtual void deletePriorObjects(double boundary,
                                    ObjectConsumer * uActiveCons)=0;
    virtual void pushBack(Object const * object,
                          bool matchedPreviously=false)=0;
    virtual bool testObject(Object const * testObject,
                            double upperLimitOnDistance,
                            ObjectPairConsumer * matchedConsumer)=0;
    virtual void clear(ObjectConsumer * uActiveCons)=0;
    virtual void finished(ObjectConsumer * uActiveCons)=0;
    virtual bool isEmpty()=0;
    virtual ActiveObject * popFront()=0;

    void setProfiler(Profiler * prof);

  protected:
    Profiler * profiler;

    ActiveList();

  private:
    ActiveList(ActiveList const &);
    ActiveList & operator=(ActiveList const &);
};

#endif // ifndef ACTIVELIST_DEFINED

