/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * NestedLoopFilter implements Filter using a simple nested
 * loop algorithm.  This is not supposed to be efficient.  It is our
 * baseline implementation against which we check the correctness and
 * compare the efficiency of other implementations.  It's not advisable
 * to run this filter on large data sets.
 */

#ifndef NESTEDLOOPFILTER_DEFINED
#define NESTEDLOOPFILTER_DEFINED

#include <list>
#include "String.h"

#include "Filter.h"


class NestedLoopFilter : public Filter
{
  public:
    NestedLoopFilter(Matcher * matcher);
    virtual ~NestedLoopFilter();
    virtual String const & getName() const { return name(); };
    virtual void filter();

    static String const & name() { return s_name; };

  private:
    static String s_name;

    NestedLoopFilter();
    NestedLoopFilter(NestedLoopFilter const &);
    NestedLoopFilter & operator=(NestedLoopFilter const &);
};

#endif // ifndef NESTEDLOOPFILTER_DEFINED

