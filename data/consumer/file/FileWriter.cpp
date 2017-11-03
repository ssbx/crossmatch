/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "FileHeader.h"
#include "FileObjectWriter.h"
#include "FileWriter.h"
#include "FileUtil.h"


FileWriter::FileWriter(FileObjectWriter * w, char const * name, bool isL)
    : ObjectWriter(),
      writer(w),
      filePtr(NULL),
      fileName(name),
      isLarge(isL)
{
}

FileWriter::~FileWriter()
{
  if (writer != 0)
    delete writer;
}

void FileWriter::open()
{
  filePtr = FileUtil::instance()->writeOpen(fileName, isLarge);
}

void FileWriter::close()
{
  FileUtil::instance()->close(filePtr);
}
