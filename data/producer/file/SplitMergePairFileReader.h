/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Reads objects from a collection of files, the same
 * as a SplitFileReader, except allows for overlaps in
 * declination order between adjacent files. Note only
 * between adjacent files.
 *
 * Specifically written to support the 2MASS collection
 * of files.
 */

#ifndef SPLITMERGEPAIRFILEREADER_DEFINED
#define SPLITMERGEPAIRFILEREADER_DEFINED

#include <stdio.h>
#include "String.h"
#include <vector>

#include "FileHeader.h"
#include "SplitFileReader.h"

class Object;
class FileObjectReader;


class SplitMergePairFileReader : public SplitFileReader
{
  public:
    SplitMergePairFileReader(FileObjectReader * obj,
                             std::vector<String> const & fileNames,
                             bool areLargeFiles=false);
    virtual ~SplitMergePairFileReader();
    virtual void open();
    virtual void reopen();
    virtual Object * read();

  protected:
    FILE *       m_next_fp;
    Object *     m_current;
    Object *     m_next;
    char const * m_nextFileName;

  private:
    SplitMergePairFileReader();
    SplitMergePairFileReader(SplitMergePairFileReader const &);
    SplitMergePairFileReader & operator=(SplitMergePairFileReader const &);

    void readyNext();
    void clear();
};

#endif // ifndef SPLITMERGEPAIRFILEREADER_DEFINED
