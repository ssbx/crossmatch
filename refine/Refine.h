/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Refine defines the interface that every refine step must implement.
 *
 * Refine classes test candidate pairs previously matched by a filter
 * for a more discriminating matching test. Two ObjectPairConsumers
 * are required to report those that do match and those that dont.
 *
 * Subclasses need to implement the refine method.
 *
 * The getName method returns the name of the refine class.
 */

#ifndef REFINE_DEFINED
#define REFINE_DEFINED

#include "String.h"

class Object;
class ObjectPairConsumer;


class Refine
{
  public:
    Refine(ObjectPairConsumer * mPPCons, ObjectPairConsumer * uPPCons);
    virtual ~Refine() {}
    virtual String const & getName() const=0;
    virtual bool refine(Object const * a, Object const * b)=0;
    virtual void finished();

  protected:
    ObjectPairConsumer * matchedObjectPairConsumer;
    ObjectPairConsumer * unmatchedObjectPairConsumer;

    virtual bool reportMatch(Object const * a, Object const * b);
    virtual void reportNoMatch(Object const * a, Object const * b);

  private:
    Refine();
    Refine(Refine const &);
    Refine & operator=(Refine const &);
};


#endif // ifndef REFINE_DEFINED
