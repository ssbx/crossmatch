/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A factory of ObjectProducers that read objects from files.
 *
 * The files may be a single file, or multiple (split) files,
 * with binary or ascii content.
 *
 * This is a singleton class, call using FileProducerFactory::instance().
 *
 * The new object producers returned must be deleted by the caller.
 */

#ifndef FILEPRODUCERFACTORY_DEFINED
#define FILEPRODUCERFACTORY_DEFINED

#include "String.h"
#include <vector>

#include "Destroyer.h"
#include "FileHeader.h"

class ObjectProducer;


class FileProducerFactory
{
  public:
    static FileProducerFactory * const instance()
    {
      if (s_instance == 0)
      {
        s_instance = new FileProducerFactory();
        DestroyerInit(s_instance)
      }
      return s_instance;
    }
    DestroyerFriend(FileProducerFactory)

    ~FileProducerFactory() {};

    ObjectProducer * createProducer(char * dir, char * prefix,
                                    bool isBinary, bool isMerge, bool isLarge,
                                    double maxSD);

    ObjectProducer * createProducer(char * dir, char * prefix,
                                    bool isBinary, bool isMerge, bool isLarge,
                                    double orthoSD, double decSD);

    ObjectProducer * createProducer(char * dir, char * prefix,
                                    bool isBinary, bool isMerge, bool isLarge,
                                    bool setMaxSDs=false);

  private:
    DestroyerDefine(FileProducerFactory)
    static FileProducerFactory * s_instance;

    FileProducerFactory() {};
    FileProducerFactory(FileProducerFactory const &);
    FileProducerFactory & operator=(FileProducerFactory const &);

    ObjectProducer * createSplitProducer(char * dir, char * prefix,
                                         bool isBinary, bool isMerge, bool isLarge,
                                         bool setMaxSDs, unsigned short numSDs=0,
                                         double maxSD=0.0,
                                         double orthoSD=0.0, double decSD=0.0);

    ObjectProducer * createProducer(char * name, bool isBinary, bool isLarge,
                                    bool setMaxSDs, unsigned short numSDs=0,
                                    double maxSD=0.0,
                                    double orthoSD=0.0, double decSD=0.0);
};

#endif // ifndef FILEPRODUCERFACTORY_DEFINED
