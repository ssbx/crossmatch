/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <vector>
#include "String.h"

#include "FileObjectReader.h"
#include "FileHeader.h"
#include "SplitFileReader.h"
#include "SplitMergePairFileReader.h"


SplitMergePairFileReader::SplitMergePairFileReader(
                                FileObjectReader * obj,
                                std::vector<String> const & fileNames,
                                bool areLargeFiles)
    : SplitFileReader(obj, fileNames, areLargeFiles),
      m_next_fp(NULL),
      m_current(0),
      m_next(0),
      m_nextFileName(0)
{
}

SplitMergePairFileReader::~SplitMergePairFileReader()
{
  clear();
}

void SplitMergePairFileReader::readyNext()
{
  if (m_next != 0)
  {
    m_current = m_next;
    fileName = m_nextFileName;
    filePtr = m_next_fp;
  }

  // open the next file, being careful with the file pointers and names
  FILE * tmp_fp = filePtr;
  char const * tmpFileName = fileName;
  SplitFileReader::open();
  m_next_fp = filePtr;
  m_nextFileName = fileName;
  filePtr = tmp_fp;
  fileName = tmpFileName;

  if (!m_finished)
  {

    // get the first object from the next file
    // assumes there is a point to be read
    // note not calling super class read method
    m_next = obj->read(m_next_fp);
    if (m_next == 0)
    {
      std::cerr << "Problem in SplitMergePairFileReader::readyNext()" << std::endl;
    }
  }
  else
  {
    // reset - the end will be determined later
    m_finished = false;
    m_next_fp = NULL;
    m_next = 0;
  }
}

void SplitMergePairFileReader::open()
{
  SplitFileReader::open();
  readyNext();
}

void SplitMergePairFileReader::clear()
{
  if (m_current != 0)
    delete m_current;
  if (m_next != 0)
    delete m_next;
  if (m_next_fp != NULL)
    ::fclose(m_next_fp);

  m_current = 0;
  m_next = 0;
  m_next_fp = NULL;
}

void SplitMergePairFileReader::reopen()
{
  m_fileNum = 0;
  m_finished = false;
  close();
  clear();
  open();
}

Object * SplitMergePairFileReader::read()
{
  // return the lower of m_current and m_next
  Object * result = 0;

  if (m_current == 0)
    m_current = obj->read(filePtr);

  if (m_current == 0)           // current file finished
  {
    if (m_next == 0)            // no next file
    {
      m_finished = true;        // now we are finished
    }
    else                        // get the next file ready
    {                           // closing current one first
      close();
      readyNext();
      result = m_current;
      m_current = 0;
    }
  }
  else if (m_next == 0)
  {
    result = m_current;
    m_current = 0;
  }
  else
  {
    if (m_current->getDec() < m_next->getDec())
    {
      result = m_current;
      m_current = 0;
    }
    else
    {
      result = m_next;
      m_next = obj->read(m_next_fp);
      if (m_next == 0)
      {
        std::cerr << "Problem in SplitMergePairFileReader::next()" << std::endl;
      }
    }
  }

  return result;
}
