/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A writer for an object.
 *
 * Default behaviour is to do nothing.
 *
 * This class defines the interface ObjectWriters must support.
 */

#ifndef OBJECTWRITER_DEFINED
#define OBJECTWRITER_DEFINED

class Object;


class ObjectWriter
{
  public:
    virtual ~ObjectWriter() {};

    virtual void open() {};
    virtual void close() {};
    virtual void write(Object const *) {};

  protected:
    ObjectWriter() {};

  private:
    ObjectWriter(ObjectWriter const & other);
    ObjectWriter & operator=(ObjectWriter const &);
};

#endif // ifndef OBJECTWRITER_DEFINED

