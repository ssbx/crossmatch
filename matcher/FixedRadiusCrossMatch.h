/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * Implements the Matcher interface for catalogue cross matching,
 * using a fixed radius great circle distance between object pairs.
 *
 * Implements the generic matcher methods required for various filter
 * algorithms. Also provides an efficient plane sweep filter.
 *
 * Two object producers are required, plus two object consumers to report
 * those objects that don't match and a pair consumer for those that do.
 *
 * The active list is required to maintain an efficient set of active
 * objects for testing against the test obejcts.
 *
 * The producers and active list are owned by this class and are deleted
 * by the destructor.
 */

#ifndef FIXEDRADIUSCROSSMATCH_DEFINED
#define FIXEDRADIUSCROSSMATCH_DEFINED

#include "String.h"

#include "ActiveList.h"
#include "CrossMatch.h"
#include "ObjectConsumer.h"

class Object;
class ObjectPairConsumer;
class ObjectProducer;


class FixedRadiusCrossMatch : public CrossMatch
{
  public:
    FixedRadiusCrossMatch(ObjectProducer * activeProd,
                          ObjectProducer * testProd,
                          ActiveList * activeList,
                          ObjectPairConsumer * pairCons,
                          ObjectConsumer * uActiveCons,
                          ObjectConsumer * uTestCons,
                          double maxD);
    virtual ~FixedRadiusCrossMatch();

    virtual String const & getName() const { return name(); };
    static String const & name() { return s_name; };

    virtual double getLowerBound(Object const * testObject);
    virtual double getUpperBound(Object const * testObject);

  protected:
    virtual double setAngularUpperLimit(Object const *) const
                        { return maxDistance; }

  private:
    static String const s_name;

    double           maxDistance;

    FixedRadiusCrossMatch();
    FixedRadiusCrossMatch(FixedRadiusCrossMatch const &);
    FixedRadiusCrossMatch & operator=(FixedRadiusCrossMatch const &);
};


#endif // ifndef FIXEDRADIUSCROSSMATCH_DEFINED
