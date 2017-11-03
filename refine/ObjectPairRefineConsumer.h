/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * This is a object pair consumer that includes a Refine object
 * to determine how objects should be consumed.
 *
 * The refine class includes the object pair consumers (which themselves
 * contain ObjectPairWriters) to consume the object pairs depending if
 * the object are accepted or not.
 *
 * For this reason, this class does not need to provide an ObjectPairWriter,
 * since the writing of objects is handled by the ObjectPairConsumers in
 * the Refine class.
 */

#ifndef OBJECTPAIRREFINECONSUMER_DEFINED
#define OBJECTPAIRREFINECONSUMER_DEFINED

#include "String.h"

#include "ObjectPairConsumer.h"

class Refine;


class ObjectPairRefineConsumer : public ObjectPairConsumer
{
  public:
    ObjectPairRefineConsumer(String const & name,
                             Refine * refine);
    virtual ~ObjectPairRefineConsumer() {};
    virtual bool report(Object const * redObject, Object const * blueObject);
    virtual void finished();

  private:
    Refine * m_refine;

    void reset();

    ObjectPairRefineConsumer();
    ObjectPairRefineConsumer(ObjectPairRefineConsumer const &);
    ObjectPairRefineConsumer & operator=(ObjectPairRefineConsumer const &);
};

#endif // ifndef OBJECTPAIRREFINECONSUMER_DEFINED
