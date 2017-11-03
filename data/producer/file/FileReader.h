/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Reads objects from a file.
 *
 * The FileObjectReader is used to hide the details of the
 * object being read. This allows this same class to be used
 * to read different kinds of objects (Object, Datum, ...) from
 * different file types (ascii, binary, ...).
 *
 * The FileObjectReader is owned by this class and is deleted 
 * by the destructor.
 */

#ifndef FILEREADER_DEFINED
#define FILEREADER_DEFINED

#include "FileObjectReader.h"
#include "ObjectReader.h"

class Object;


class FileReader : public ObjectReader
{
  public:
    FileReader(FileObjectReader * obj, char const * name, bool isLarge=false);
    virtual ~FileReader();

    virtual void open();
    virtual void reopen();
    virtual void close();
    virtual Object * read() { return obj->read(filePtr); };

  protected:
    FileObjectReader * obj;
    FILE *             filePtr;
    char const *       fileName;
    bool               isLarge;

    FileReader(FileObjectReader * obj, bool isLarge=false);

  private:
    FileReader();
    FileReader(FileReader const & other);
    FileReader & operator=(FileReader const &);
};

#endif // ifndef FILEREADER_DEFINED

