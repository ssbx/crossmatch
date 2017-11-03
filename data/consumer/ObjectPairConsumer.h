/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A consumer of object pairs. This class counts the number
 * of object pairs reported and forwards them to the pair writer
 * provided to the constructir, if any.
 *
 * When finished called, the number of object pairs processed
 * is printed to stdout.
 *
 * The default behaviour is to write out the object pairs as they
 * are reported. For further refinement, refer to the class
 * ObjectPairRefineConsumer, which includes a further refinement
 * step that needs to be successful before writing is performed.
 *
 * This class should be used to count (and possible write) those
 * pairs of objects that fail to meet the further refinement
 * criteria.
 *
 * Refer to the class Refine class for details.
 *
 * This class owns the ObjectPairWriter and deletes it in the destructor.
 */

#ifndef OBJECTPAIRCONSUMER_DEFINED
#define OBJECTPAIRCONSUMER_DEFINED

#include "ObjectPairWriter.h"
#include "String.h"

class Object;


class ObjectPairConsumer
{
  public:
    ObjectPairConsumer(String const & name,
                       ObjectPairWriter * writer=0);
    virtual ~ObjectPairConsumer();
    String const & getName() const { return m_name; };
    virtual bool report(Object const * redObject, Object const * blueObject)
    {
      if (m_writer != 0)
        m_writer->write(redObject, blueObject);
      m_count++;
      return true;
    }
    virtual void finished();

  protected:
    ObjectPairWriter * m_writer;
    unsigned long long m_count;

  private:
    String m_name;

    ObjectPairConsumer(ObjectPairConsumer const &);
    ObjectPairConsumer & operator=(ObjectPairConsumer const &);
};

#endif // ifndef OBJECTPAIRCONSUMER_DEFINED

