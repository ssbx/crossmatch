/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A writer for a pair of objects.
 * Default behaviour is to do nothing.
 * This class defines the interface ObjectPairWriters must support.
 */

#ifndef OBJECTPAIRWRITER_DEFINED
#define OBJECTPAIRWRITER_DEFINED

#include "Object.h"


class ObjectPairWriter
{
  public:
    ObjectPairWriter() {};
    virtual ~ObjectPairWriter() {};
    virtual void open() {};
    virtual void close() {};
    virtual void write(Object const *, Object const *) {};

  private:
    ObjectPairWriter(ObjectPairWriter const & other);
    ObjectPairWriter & operator=(ObjectPairWriter const &);
};

#endif // ifndef OBJECTPAIRWRITER_DEFINED

