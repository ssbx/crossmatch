/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Filter is the interface that every filter implementation must instantiate.
 *
 * A filter is created with a matcher - this defines the logic for
 * efficiently processing candidates identified by the filter.
 *
 * Each filter subclass implements a standard filter method. This
 * is the generic algorithm that the matcher must support. However,
 * the matcher may have an optimised filter operation. This can be
 * called using the matchFilter method. Note - this "fast" matcher
 * provided filter will thus bypass the Filter subclass means of 
 * idenifying candidates for refinement. For example, if the "fast" 
 * cross match filter is used with the nested loop filter, then
 * it isn'
 *
 * The matcher is owned by this class and is deleted by the destructor.
 */

#ifndef FILTER_DEFINED
#define FILTER_DEFINED

#include "String.h"

class Matcher;


class Filter
{
  public:
    Filter(Matcher * matcher);

    virtual ~Filter();
    virtual String const & getName();
    virtual void filter();

  protected:
    Matcher * matcher;

    virtual void preFilter();
    virtual void postFilter();

  private:
    static String s_name;

    Filter(Filter const &);
    Filter & operator=(Filter const &);
};


#endif // ifndef FILTER_DEFINED
