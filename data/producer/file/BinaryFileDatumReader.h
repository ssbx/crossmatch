/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Reads the contents of an object to a file using the read method.
 *
 * The object is a Datum and is read from a binary file.
 */

#ifndef BINARYFILEDATUMREADER_DEFINED
#define BINARYFILEDATUMREADER_DEFINED

#include "FileObjectReader.h"
#include "FileHeader.h"

class Object;


class BinaryFileDatumReader : public FileObjectReader
{
  public:
    BinaryFileDatumReader();
    virtual ~BinaryFileDatumReader() {};
    virtual Object * read(FILE * fp);

  private:
    BinaryFileDatumReader(BinaryFileDatumReader const & other);
    BinaryFileDatumReader & operator=(BinaryFileDatumReader const &);
};

#endif // ifndef BINARYFILEDATUMREADER_DEFINED

