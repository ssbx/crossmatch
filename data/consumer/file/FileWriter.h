/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Writes objects to a file.
 *
 * The FileObjectWriter is used to hide the details of the
 * object being written. This allows this same class to be used
 * to write different kinds of objects (Object, Datum, ...) to 
 * different file types (ascii, binary, ...).
 */

#ifndef FILEWRITER_DEFINED
#define FILEWRITER_DEFINED

#include "FileObjectWriter.h"
#include "FileUtil.h"
#include "ObjectWriter.h"

class Object;


class FileWriter : public ObjectWriter
{
  public:
    FileWriter(FileObjectWriter * w, char const * name, bool isLarge=false);
    virtual ~FileWriter();

    virtual void open();
    virtual void close();
    virtual void write(Object const * object) { return writer->write(filePtr, object); };

  protected:
    FileObjectWriter * writer;
    FILE *             filePtr;
    char const *       fileName;
    bool               isLarge;

  private:
    FileWriter();
    FileWriter(FileWriter const & other);
    FileWriter & operator=(FileWriter const &);
};

#endif // ifndef FILEWRITER_DEFINED

