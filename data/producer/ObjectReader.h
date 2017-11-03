/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A reader for an object.
 *
 * Default behaviour is to do nothing.
 *
 * This class defines the interface ObjectReaders must support.
 */

#ifndef OBJECTREADER_DEFINED
#define OBJECTREADER_DEFINED

#include "Object.h"


class ObjectReader
{
  public:
    virtual ~ObjectReader() {};

    virtual void open()=0;
    virtual void reopen()=0;
    virtual void close()=0;
    virtual Object * read()=0;

  protected:
    ObjectReader() {};

  private:
    ObjectReader(ObjectReader const & other);
    ObjectReader & operator=(ObjectReader const &);
};

#endif // ifndef OBJECTREADER_DEFINED

