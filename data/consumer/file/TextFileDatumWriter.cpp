/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "Datum.h"
#include "FileHeader.h"
#include "FileObjectWriter.h"
#include "TextFileDatumWriter.h"

class Object;


TextFileDatumWriter::TextFileDatumWriter()
    : FileObjectWriter()
{
}

void TextFileDatumWriter::write(FILE * fp, Object const * object)
{
  Datum const * datum = (Datum const *) object;

  fprintf(fp, "%llu %.10f %.10f %.10f %.10f\n",
                datum->getId(),
                datum->getRA(),
                datum->getOrthoSD(),
                datum->getDec(),
                datum->getDecSD());
}
