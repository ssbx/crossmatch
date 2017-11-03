/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "FileHeader.h"
#include "FileObjectWriter.h"
#include "FilePairWriter.h"
#include "FileUtil.h"


FilePairWriter::FilePairWriter(FileObjectWriter * w, char const * name, bool isL)
    : ObjectPairWriter(),
      writer(w),
      filePtr(NULL),
      fileName(name),
      isLarge(isL)
{
}

FilePairWriter::~FilePairWriter()
{
  if (writer != 0)
    delete writer;
}

void FilePairWriter::open()
{
  filePtr = FileUtil::instance()->writeOpen(fileName, isLarge);
}

void FilePairWriter::close()
{
  FileUtil::instance()->close(filePtr);
}
