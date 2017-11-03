/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * DecPlaneSweep implements Filter using a plane sweep algorithm
 * in which the sweep line travels along the declination axis
 * from -90 degrees to 90 degrees.
 */

#ifndef DECPLANESWEEPFILTER_DEFINED
#define DECPLANESWEEPFILTER_DEFINED

#include "String.h"

#include "Filter.h"

class Matcher;


class DecPlaneSweepFilter : public Filter
{
  public:
    DecPlaneSweepFilter(Matcher * matcher);
    virtual ~DecPlaneSweepFilter();
    virtual String const & getName() const { return name(); };
    virtual void filter();

    static String const & name() { return s_name; };

  private:
    static String s_name;

    DecPlaneSweepFilter();
    DecPlaneSweepFilter(DecPlaneSweepFilter const &);
    DecPlaneSweepFilter & operator=(DecPlaneSweepFilter const &);
};

#endif // ifndef DECPLANESWEEPFILTER_DEFINED
