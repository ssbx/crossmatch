/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Reads objects from a collection of files.
 *
 * The file names are provided as a vector of Strings
 * and each file is opened in turn.
 *
 * Assumes the files are in order of increasing declination 
 * within and BETWEEN subsequent files.
 */

#ifndef SPLITFILEREADER_DEFINED
#define SPLITFILEREADER_DEFINED

#include <stdio.h>
#include "String.h"
#include <vector>

#include "FileReader.h"

class FileObjectReader;


class SplitFileReader : public FileReader
{
  public:
    SplitFileReader(FileObjectReader * obj,
                    std::vector<String> const & fileNames,
                    bool areLargeFiles=false);
    virtual ~SplitFileReader();
    virtual void open();
    virtual void reopen();
    virtual Object * read();

  protected:
    std::vector<String> m_fileNames;
    unsigned int        m_fileNum;
    bool                m_finished;

  private:
    SplitFileReader();
    SplitFileReader(SplitFileReader const &);
    SplitFileReader & operator=(SplitFileReader const &);
};

#endif // ifndef SPLITFILEREADER_DEFINED
