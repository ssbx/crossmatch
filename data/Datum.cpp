/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <iostream>

#include "Datum.h"
#include "Object.h"


Datum::Datum(unsigned long long i, double r, double d, double sigma)
    : Object(r, d),
      id(i),
      orthoSD(sigma),
      decSD(sigma),
      validSD(true),
      sd(sigma)
{
}

Datum::Datum(unsigned long long i, double r, double rSD, double d, double dSD)
    : Object(r, d),
      id(i),
      orthoSD(rSD),
      decSD(dSD),
      validSD(false),
      sd(0.0)
{
}

double Datum::getSD() const
{
  if (!validSD)
  {
    sd = (orthoSD > decSD) ? orthoSD : decSD;
    validSD = true;
  }

  return sd;
}

void Datum::print(std::ostream & os) const
{
  os << getId() << " ";

  Object::print(os);
}
