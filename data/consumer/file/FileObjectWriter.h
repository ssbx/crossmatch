/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Writes the contents of an object to a file using the write method.
 *
 * This class defines the interface all FileObjectWriters must implement.
 *
 * Refere to subclasses for examples.
 */

#ifndef FILEOBJECTWRITER_DEFINED
#define FILEOBJECTWRITER_DEFINED

#include "FileHeader.h"

class Object;


class FileObjectWriter
{
  public:
    virtual ~FileObjectWriter() {};
    virtual void write(FILE * fp, Object const * object)=0;

  protected:
    FileObjectWriter() {};

  private:
    FileObjectWriter(FileObjectWriter const & other);
    FileObjectWriter & operator=(FileObjectWriter const &);
};

#endif // ifndef FILEOBJECTWRITER_DEFINED

