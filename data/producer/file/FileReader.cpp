/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "FileHeader.h"
#include "FileObjectReader.h"
#include "FileReader.h"
#include "FileUtil.h"


FileReader::FileReader(FileObjectReader * o, char const * name, bool isL)
    : ObjectReader(),
      obj(o),
      filePtr(NULL),
      fileName(name),
      isLarge(isL)
{
}

FileReader::FileReader(FileObjectReader * o, bool isL)
    : ObjectReader(),
      obj(o),
      filePtr(NULL),
      fileName(0),
      isLarge(isL)
{
}

FileReader::~FileReader()
{
  if (obj != 0)
    delete obj;
}

void FileReader::open()
{
  filePtr = FileUtil::instance()->readOpen(fileName, isLarge);
}

void FileReader::reopen()
{
  fseek(filePtr, 0, 0);
}

void FileReader::close()
{
  FileUtil::instance()->close(filePtr);
}
