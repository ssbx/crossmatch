/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <iostream>

#include "Constants.h"
#include "Object.h"

double Object::zAlpha = 1.0;


Object::Object(double r, double d)
    : ra(r), dec(d)
{
}

void Object::print(std::ostream & os) const
{
  os << getRA() << " " << getDec() << " " << getSD() << " "
     << getOrthoSD() << " " << getDecSD();
}

double Object::computeDistanceBound(double redSD,
                                    double blueSD)
{
  return zAlpha * std::sqrt(redSD * redSD + blueSD * blueSD);
}

bool Object::computeRACorrection(double radius, double dec, double & limit)
{
  bool withinBounds = true;

  if (dec - radius > -90.0 && dec + radius < 90.0)
    limit = Constants::asinDeg(Constants::sinDeg(radius)/Constants::cosDeg(dec));
  else
    withinBounds = false;

  return withinBounds;
}

/*
 * Calculates the great circle distance between two points.
 */
double Object::gcDistance(Object const & other) const
{
  double distance = 0.0;

  if ((getDec() != other.getDec()) ||
      (getRA()  != other.getRA()))
  {
    distance = haversineDistance(other);
  }

  return distance;
}

/*
 * Calculates the great circle distance between two points.
 */
double Object::haversineDistance(Object const & other) const
{
  double distance = 0.0;

  /* The haversine method */
  double sinTerm1 = Constants::sinDeg((getDec() - other.getDec())*0.5);
  double cosTerm1 = Constants::cosDeg(getDec());
  double cosTerm2 = Constants::cosDeg(other.getDec());
  double sinTerm2 = Constants::sinDeg((getRA() - other.getRA())*0.5);

  double term = std::sqrt(sinTerm1*sinTerm1 + cosTerm1*cosTerm2*sinTerm2*sinTerm2);

  if (term >= 1.0)
    distance = 180;
  else
    distance = 2.0 * Constants::asinDeg(term);

  /* The cos method - less accurate for small angles */
/*
  double cosDistance = Constants::sinDeg(getDec())
                     * Constants::sinDeg(other.getDec())
                     + Constants::cosDeg(getDec())
                     * Constants::cosDeg(other.getDec())
                     * Constants::cosDeg(getRA() - other.getRA());

  distance = Constants::acosDeg(cosDistance);
*/

  return distance;
}

/* checks whether the angular separation between two objects is sufficiently
 * small that we cannot rule out the possibility that the two objects are
 * in fact spatially co-located.
 */
bool Object::isSmallSeparation(Object const & other) const
{
  bool isClose = false;
  if ((getDec() == other.getDec()) &&
      (getRA()  == other.getRA()))
  {
    isClose = true;
  }
  else
  {
    double distanceBound = computeDistanceBound(getSD(), other.getSD());

    double distance = haversineDistance(other);

    isClose = (distance <= distanceBound);
  }

  return isClose;
}

std::ostream & operator<<(std::ostream & os, Object const & obj)
{
  obj.print(os);

  return os;
}
