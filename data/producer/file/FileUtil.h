/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Common file operations. Opening a file for reading or writing,
 * closing a file, and reading the filenames in a given directory
 * with a common prefix.
 *
 * This is a singleton class. Call using FileUtil::instance().
 */

#ifndef FILEUTIL_DEFINED
#define FILEUTIL_DEFINED

#include "String.h"
#include <vector>

#include "Destroyer.h"
#include "FileHeader.h"


class FileUtil
{
  public:
    static FileUtil * const instance()
    {
      if (s_instance == 0)
      {
        s_instance = new FileUtil();
        DestroyerInit(s_instance)
      }
      return s_instance;
    }
    DestroyerFriend(FileUtil)

    ~FileUtil() {};

    void readDir(std::vector<String> & files, char * dirName, char * prefix);

    FILE * readOpen(String const & fileName, bool isLarge=false);
    FILE * writeOpen(String const & fileName, bool isLarge=false);
    void close(FILE * & fp);

  private:
    DestroyerDefine(FileUtil)
    static FileUtil * s_instance;

    FileUtil() {};
    FileUtil(FileUtil const &);
    FileUtil & operator=(FileUtil const &);
};

#endif // ifndef FILEUTIL_DEFINED
