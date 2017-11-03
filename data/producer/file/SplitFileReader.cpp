/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <stdio.h>
#include "String.h"
#include <vector>

#include "FileObjectReader.h"
#include "FileReader.h"
#include "SplitFileReader.h"


SplitFileReader::SplitFileReader(FileObjectReader * obj,
                                 std::vector<String> const & fileNames,
                                 bool areLargeFiles)
    : FileReader(obj, areLargeFiles),
      m_fileNames(fileNames),
      m_fileNum(0),
      m_finished(false)
{
}

SplitFileReader::~SplitFileReader()
{
  m_fileNames.clear();
}

void SplitFileReader::open()
{
  if (m_fileNum < m_fileNames.size())
  {
    fileName = m_fileNames[m_fileNum++].c_str();
    FileReader::open();
  }
  else
  {
    m_finished = true;
  }
}

void SplitFileReader::reopen()
{
  m_fileNum = 0;
  m_finished = false;
  close();
  open();
}

Object * SplitFileReader::read()
{
  Object * next = 0;

  if (!m_finished)
  {
    next = FileReader::read();
    if (next == 0)
    {
      // try to read from the next file
      close();
      open();
      if (!m_finished)
      {
        next = read();
      }
    }
  }

  return next;
}
