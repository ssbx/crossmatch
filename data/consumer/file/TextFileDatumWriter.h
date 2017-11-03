/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Writes the contents of an object to a file using the write method.
 *
 * The object is a Datum and is written to a text file. Useful for
 * debugging purposes.
 */

#ifndef TEXTFILEDATUMWRITER_DEFINED
#define TEXTFILEDATUMWRITER_DEFINED

#include "FileHeader.h"
#include "FileObjectWriter.h"

class Object;


class TextFileDatumWriter : public FileObjectWriter
{
  public:
    TextFileDatumWriter();
    virtual ~TextFileDatumWriter() {};
    virtual void write(FILE * fp, Object const * object);

  private:
    TextFileDatumWriter(TextFileDatumWriter const & other);
    TextFileDatumWriter & operator=(TextFileDatumWriter const &);
};

#endif // ifndef TEXTFILEDATUMWRITER_DEFINED

