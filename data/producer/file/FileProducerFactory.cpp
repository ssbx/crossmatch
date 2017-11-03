/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "String.h"
#include "string.h"
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

#include "BinaryFileDatumReader.h"
#include "Destroyer.h"
#include "FileProducerFactory.h"
#include "FileObjectReader.h"
#include "FileUtil.h"
#include "ObjectProducer.h"
#include "SplitFileReader.h"
#include "SplitMergePairFileReader.h"
#include "TextFileDatumReader.h"


FileProducerFactory * FileProducerFactory::s_instance = 0;
DestroyerDeclare(FileProducerFactory)


ObjectProducer * FileProducerFactory::createProducer(char * dir,
                                                     char * name,
                                                     bool isBinary,
                                                     bool isMerge,
                                                     bool isLarge,
                                                     double maxSD)
{
  ObjectProducer * prod = 0;
  if (dir == 0 || strcmp(dir, "0") == 0)
  {
    prod = createProducer(name, isBinary, isLarge, false, 1, maxSD);
  }
  else
  {
    prod = createSplitProducer(dir, name, isBinary, isMerge, isLarge, false, 1, maxSD);
  }

  return prod;
}

ObjectProducer * FileProducerFactory::createProducer(char * dir,
                                                     char * name,
                                                     bool isBinary,
                                                     bool isMerge,
                                                     bool isLarge,
                                                     double orthoSD,
                                                     double decSD)
{
  ObjectProducer * prod = 0;
  if (dir == 0 || strcmp(dir, "0") == 0)
  {
    prod = createProducer(name, isBinary, false, isLarge, 2, 0.0, orthoSD, decSD);
  }
  else
  {
    prod = createSplitProducer(dir, name, isBinary, isMerge, isLarge, false, 2, 0.0, orthoSD, decSD);
  }

  return prod;
}

ObjectProducer * FileProducerFactory::createProducer(char * dir,
                                                     char * name,
                                                     bool isBinary,
                                                     bool isMerge,
                                                     bool isLarge,
                                                     bool setMaxSDs)
{
  ObjectProducer * prod = 0;
  if (dir == 0 || strcmp(dir, "0") == 0)
  {
    prod = createProducer(name, isBinary, isLarge, setMaxSDs);
  }
  else
  {
    prod = createSplitProducer(dir, name, isBinary, isMerge, isLarge, setMaxSDs);
  }

  return prod;
}

ObjectProducer * FileProducerFactory::createSplitProducer(char * dir,
                                                          char * prefix,
                                                          bool isBinary,
                                                          bool isMerge,
                                                          bool isLarge,
                                                          bool setMaxSDs,
                                                          unsigned short numSDs,
                                                          double maxSD,
                                                          double orthoSD,
                                                          double decSD)
{
  ObjectProducer * pp = 0;

  std::vector<String> fileNames;
  FileUtil::instance()->readDir(fileNames, dir, prefix);

  FileObjectReader * fileObj =
        (isBinary) ? (FileObjectReader *) new BinaryFileDatumReader()
                   : (FileObjectReader *) new TextFileDatumReader();
  ObjectReader * objReader = (isMerge) ? new SplitMergePairFileReader(fileObj, fileNames, isLarge)
                                       : new SplitFileReader(fileObj, fileNames, isLarge);

  if (numSDs == 0)
    pp = new ObjectProducer(objReader, setMaxSDs);
  else if (numSDs == 1)
    pp = new ObjectProducer(objReader, maxSD);
  else
    pp = new ObjectProducer(objReader, orthoSD, decSD);

  return pp;
}

ObjectProducer * FileProducerFactory::createProducer(char * name,
                                                     bool isBinary,
                                                     bool isLarge,
                                                     bool setMaxSDs,
                                                     unsigned short numSDs,
                                                     double maxSD,
                                                     double orthoSD,
                                                     double decSD)
{
  ObjectProducer * pp = 0;

  FileObjectReader * fileObj = 
        (isBinary) ? (FileObjectReader *) new BinaryFileDatumReader()
                   : (FileObjectReader *) new TextFileDatumReader();
  ObjectReader * objReader = new FileReader(fileObj, name, isLarge);

  if (numSDs == 0)
    pp = new ObjectProducer(objReader, setMaxSDs);
  else if (numSDs == 1)
    pp = new ObjectProducer(objReader, maxSD);
  else
    pp = new ObjectProducer(objReader, orthoSD, decSD);

  return pp;
}
