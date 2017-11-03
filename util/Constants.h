/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#ifndef CONSTANTS_DEFINED
#define CONSTANTS_DEFINED

#include <cmath>

class Constants
{
  public:
    static const long double s_pi;
    static const long double s_raddeg;
    static const long double s_degrad;

    // Trig functions where the angles used are in degrees.
    // Not really constants, but oh well...
    static double sinDeg(double x)  { return std::sin(x * Constants::s_raddeg); }
    static double cosDeg(double x)  { return std::cos(x * Constants::s_raddeg); }
    static double asinDeg(double x) { return std::asin(x) * Constants::s_degrad; }
    static double acosDeg(double x) { return std::acos(x) * Constants::s_degrad; }

  private:
    Constants() {};
    ~Constants() {};
    Constants(Constants const &);
    Constants & operator=(Constants const &);
};

#endif // ifndef CONSTANTS_DEFINED
