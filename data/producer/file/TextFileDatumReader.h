/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Reads the contents of an object to a file using the read method.
 *
 * The object is a Datum and is read from a text file.
 */

#ifndef TEXTFILEDATUMREADER_DEFINED
#define TEXTFILEDATUMREADER_DEFINED

#include "FileHeader.h"
#include "FileObjectReader.h"

class Object;


class TextFileDatumReader : public FileObjectReader
{
  public:
    TextFileDatumReader();
    virtual ~TextFileDatumReader() {};
    virtual Object * read(FILE * fp);

  private:
    TextFileDatumReader(TextFileDatumReader const & other);
    TextFileDatumReader & operator=(TextFileDatumReader const &);
};

#endif // ifndef TEXTFILEDATUMREADER_DEFINED

