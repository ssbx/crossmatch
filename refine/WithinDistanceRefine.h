/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * WithinDistanceRefine is a Refine that accepts those candidate pairs
 * whose great circle distance are within the match distance specified.
 */

#ifndef WITHINDISTANCEREFINE_DEFINED
#define WITHINDISTANCEREFINE_DEFINED

#include "String.h"

#include "Object.h"
#include "Refine.h"

class ObjectPairConsumer;


class WithinDistanceRefine : public Refine
{
  public:
    WithinDistanceRefine(double maxDistance,
                         ObjectPairConsumer * mPPCons,
                         ObjectPairConsumer * uPPCons);
    virtual ~WithinDistanceRefine();
    virtual String const & getName() const { return name(); };
    virtual bool refine(Object const * red, Object const * blue);

    static String const & name() { return s_name; };

  private:
    static String s_name;

    double maxDistance;

    WithinDistanceRefine();
    WithinDistanceRefine(WithinDistanceRefine const &);
    WithinDistanceRefine & operator=(WithinDistanceRefine const &);
};

#endif // ifndef WITHINDISTANCEREFINE_DEFINED

