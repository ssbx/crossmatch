/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "Datum.h"
#include "FileHeader.h"
#include "FileObjectReader.h"
#include "TextFileDatumReader.h"

class Object;


TextFileDatumReader::TextFileDatumReader()
    : FileObjectReader()
{
}

Object * TextFileDatumReader::read(FILE * fp)
{
  Object * result = 0;
  unsigned long long id;
  double ra, orthoSD, dec, decSD;

  if (fscanf(fp, "%llu %lf %lf %lf %lf\n",
                 &id, &ra, &orthoSD, &dec, &decSD) == 5)
  {
    result = new Datum(id, ra, orthoSD, dec, decSD);
  }

  return result;
}
