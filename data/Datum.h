/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#ifndef DATUM_DEFINED
#define DATUM_DEFINED

#include <iostream>

#include "Object.h"


class Datum : public Object
{
  public:
    Datum(unsigned long long id, double ra, double dec, double sd);
    Datum(unsigned long long id, double ra, double orthoSD, double dec, double decSD);
    virtual ~Datum() {};

    virtual double getSD()      const;
    virtual double getOrthoSD() const   { return orthoSD; };
    virtual double getDecSD()   const   { return decSD; };
    virtual void print(std::ostream & os) const;

    unsigned long long getId()      const   { return id; };

  private:
    unsigned long long id;
    double orthoSD;
    double decSD;
    mutable bool validSD;
    mutable double sd;

    Datum();
    Datum(Datum const & other);
    Datum & operator=(Datum const &);
};

#endif // ifndef DATUM_DEFINED
