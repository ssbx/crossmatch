/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * AngularSeparationRefine is a Refine that accepts those candidate pairs
 * whose angular separations are such that the likelihood that the two 
 * records represent the same source is greater than some given threshold.
 */

#ifndef ANGULARSEPARATIONREFINE_DEFINED
#define ANGULARSEPARATIONREFINE_DEFINED

#include "String.h"

#include "Refine.h"
#include "Object.h"

class ObjectPairConsumer;


class AngularSeparationRefine : public Refine
{
  public:
    AngularSeparationRefine(ObjectPairConsumer * mPPCons, ObjectPairConsumer * uPPCons);
    virtual ~AngularSeparationRefine();
    virtual String const & getName() const { return name(); };
    virtual bool refine(Object const * red, Object const * blue);

    static String const & name() { return s_name; };

  private:
    static String s_name;

    AngularSeparationRefine();
    AngularSeparationRefine(AngularSeparationRefine const &);
    AngularSeparationRefine & operator=(AngularSeparationRefine const &);
};

#endif // ifndef ANGULARSEPARATIONREFINE_DEFINED

