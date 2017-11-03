/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "String.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <iostream>

#include "Destroyer.h"
#include "FileUtil.h"
#include "FileHeader.h"


FileUtil * FileUtil::s_instance = 0;
DestroyerDeclare(FileUtil)


void FileUtil::readDir(std::vector<String> & files,
                       char * dirName,
                       char * prefix)
{
  int n = 0;
  struct dirent *dp;
  DIR * dfd = ::opendir(dirName);
  int prefix_len = strlen(prefix);
  if (dfd != NULL)
  {
    char path[2048];
    while ((dp = ::readdir(dfd)) != NULL)
    {
      if (strncmp(dp->d_name, prefix, prefix_len) == 0)
      {
        n++;
        strcpy(path, dirName);
        strcat(path, "/");
        strcat(path, dp->d_name);
        String filename(path);
        files.push_back(filename);
      }
    }
    ::closedir(dfd);

    sort(files.begin(), files.end());
  }

  if (n == 0)
    fprintf(stderr, "WARNING: no files found matching: %s/%s\n", dirName, prefix);
}

FILE * FileUtil::readOpen(String const & fileName, bool isLarge)
{
  FILE * fp = NULL;

  if (isLarge)
  {
    int fd = ::open(fileName.c_str(), O_RDONLY|O_LARGEFILE, 0644);
    if (fd > 0)
    {
      fp = ::fdopen(fd, "r");
    }
  }
  else
  {
    fp = ::fopen(fileName.c_str(), "r");
  }

  if (fp == NULL)
  {
    std::cout << "EXCEPTION: problem opening file: " << fileName << std::endl;
    exit(1);
  }
  else
  {
    std::cout << "opening file: " << fileName << std::endl;
  }

  return fp;
}

FILE * FileUtil::writeOpen(String const & fileName, bool isLarge)
{
  FILE * fp = NULL;

  if (isLarge)
  {
    int fd = ::open(fileName.c_str(), O_CREAT|O_TRUNC|O_WRONLY|O_LARGEFILE, 0644);
    if (fd > 0)
    {
      fp = ::fdopen(fd, "w");
    }
  }
  else
  {
    fp = ::fopen(fileName.c_str(), "w");
  }

  if (fp == NULL)
  {
    std::cout << "EXCEPTION: problem opening file: " << fileName << std::endl;
    exit(1);
  }
  else
  {
    std::cout << "opening file: " << fileName << std::endl;
  }

  return fp;
}

void FileUtil::close(FILE * & fp)
{
  if (fp != 0)
  {
    ::fclose(fp);
    fp = 0;
  }
}
