/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * BoundingBoxRefine is a Refine that accepts those candidate pairs
 * whose bounding boxed intersect.  This is /almost/ what the DecPlaneSweepFilter
 * does, so the BoundingBoxRefine will reject only a small number of
 * candidates pairs.
 */

#ifndef BOUNDINGBOXREFINE_DEFINED
#define BOUNDINGBOXREFINE_DEFINED

#include "String.h"

#include "Refine.h"

class ObjectPairConsumer;


class BoundingBoxRefine : public Refine
{
  public:
    BoundingBoxRefine(ObjectPairConsumer * mPPCons,
                      ObjectPairConsumer * uPPCons);
    virtual ~BoundingBoxRefine();
    virtual String const & getName() const { return name(); };
    virtual bool refine(Object const * red, Object const * blue);

    static String const & name() { return s_name; };

  private:
    static String s_name;

    BoundingBoxRefine();
    BoundingBoxRefine(BoundingBoxRefine const &);
    BoundingBoxRefine & operator=(BoundingBoxRefine const &);
};

#endif // ifndef BOUNDINGBOXREFINE_DEFINED

