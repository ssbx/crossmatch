/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Writes pair of objects to a file.
 *
 * The FileObjectWriter is used to hide the details of the
 * object being written. This allows this same class to be used
 * to write different kinds of objects (Object, Datum, ...) to 
 * different file types (ascii, binary, ...).
 */

#ifndef FILEPAIRWRITER_DEFINED
#define FILEPAIRWRITER_DEFINED

#include "FileObjectWriter.h"
#include "FileUtil.h"
#include "ObjectPairWriter.h"

class Object;


class FilePairWriter : public ObjectPairWriter
{
  public:
    FilePairWriter(FileObjectWriter * w, char const * name, bool isLarge=false);
    virtual ~FilePairWriter();

    virtual void open();
    virtual void close();
    virtual void write(Object const * redObject,
                       Object const * blueObject)
    {
        writer->write(filePtr, redObject);
        writer->write(filePtr, blueObject);
    };

  protected:
    FileObjectWriter * writer;
    FILE *             filePtr;
    char const *       fileName;
    bool               isLarge;

  private:
    FilePairWriter();
    FilePairWriter(FilePairWriter const & other);
    FilePairWriter & operator=(FilePairWriter const &);
};

#endif // ifndef FILEPAIRWRITER_DEFINED

