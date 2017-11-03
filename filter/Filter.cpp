/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include <iostream>

#include "Filter.h"
#include "Matcher.h"
#include "Object.h"


String Filter::s_name("Filter");


Filter::Filter(Matcher * m)
    : matcher(m)
{
}

Filter::~Filter()
{
  delete matcher;
}

String const & Filter::getName()
{
  return matcher->getName();
}

void Filter::preFilter()
{
  matcher->getReady();
}

void Filter::filter()
{
  if (matcher->hasFilter())
  {
    preFilter();
    matcher->doFilter();
    postFilter();
  }
  else
  {
    std::cerr << "There is no fast filter for " << matcher->getName() << std::endl;
  }
}

void Filter::postFilter()
{
  matcher->finished();
}
