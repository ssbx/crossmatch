/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * An object from a catalogue which has a location
 * defined as it's RA and Dec in decimal degrees.
 * There may also be errors associated with these
 * values, defined as the standard deviation.
 * the errors may be different in the RA and Dec
 * dimensions (getOrthoSD and getDecSD), or be the same
 * for each (getSD). When different, getSD should return 
 * the max of getOrthoSD and getDecSD.
 *
 * zAlpha is the value that puts a probability of alpha
 * into the tail of a normal distribution and is used
 * in computing the distance bound between two objects.
 * A value of 1.96 corresponds to a 95% confidence that
 * two objects are not spatially coincident.
 */

#ifndef OBJECT_DEFINED
#define OBJECT_DEFINED

#include <iostream>

#include "Constants.h"


class Object
{
  public:
    Object(double ra, double dec);
    virtual ~Object() {};

    virtual double getRA()      const { return ra; };
    virtual double getDec()     const { return dec; };
    virtual double getSD()      const { return 0.0; };
    virtual double getOrthoSD() const { return 0.0; };
    virtual double getDecSD()   const { return 0.0; };
    virtual void print(std::ostream & os) const;

    double gcDistance(Object const & other) const;

    bool isSmallSeparation(Object const & other) const;

    // the zAlpha for the required confidence
    static double zAlpha;

    static double computeDistanceBound(double redSD, double blueSD);
    static bool computeRACorrection(double radius, double dec, double & limit);

  private:
    double ra;
    double dec;

    Object();
    Object(Object const & other);
    Object & operator=(Object const &);

    double haversineDistance(Object const & other) const;
};

std::ostream & operator<<(std::ostream & os, Object const & object);

#endif // ifndef OBJECT_DEFINED

