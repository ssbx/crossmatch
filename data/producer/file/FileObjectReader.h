/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Reads the contents of an object from a file using the read method.
 *
 * This class defines the interface all FileObjectReaders must implement.
 *
 * Refere to subclasses for examples.
 */

#ifndef FILEOBJECTREADER_DEFINED
#define FILEOBJECTREADER_DEFINED

#include "FileHeader.h"

class Object;


class FileObjectReader
{
  public:
    virtual ~FileObjectReader() {};
    virtual Object * read(FILE * fp)=0;

  protected:
    FileObjectReader() {};

  private:
    FileObjectReader(FileObjectReader const & other);
    FileObjectReader & operator=(FileObjectReader const &);
};

#endif // ifndef FILEOBJECTREADER_DEFINED

