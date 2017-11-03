/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "Datum.h"
#include "FileHeader.h"
#include "FileObjectReader.h"
#include "BinaryFileDatumReader.h"

class Object;


BinaryFileDatumReader::BinaryFileDatumReader()
    : FileObjectReader()
{
}

Object * BinaryFileDatumReader::read(FILE * fp)
{
  static const unsigned int BUF_SIZE = 40;
  static char buf[BUF_SIZE];

  Object * result = 0;

  if (fread(buf, sizeof(char), BUF_SIZE, fp) == BUF_SIZE)
  {
    unsigned long long id = *((unsigned long long *) (&buf[0]));
    double ra =             *((double *)             (&buf[8]));
    double orthoSD =        *((double *)             (&buf[16]));
    double dec =            *((double *)             (&buf[24]));
    double decSD =          *((double *)             (&buf[32]));

    result = new Datum(id, ra, orthoSD, dec, decSD);
  }

  return result;
}
