/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A consumer of objects. This class counts the number
 * of objects reported and forwards them to the writer
 * provided to the constructir, if any.
 *
 * When finished called, the number of objects processed
 * is printed to stdout.
 *
 * This class should not be subclassed since there is
 * nothing more to be done with "single" (unmatched)
 * objects, unlike the case for pairs of objects.
 *
 * This class owns the ObjectWriter and deletes it in the destructor.
 */

#ifndef OBJECTCONSUMER_DEFINED
#define OBJECTCONSUMER_DEFINED

#include "String.h"
#include "ObjectWriter.h"

class Object;


class ObjectConsumer
{
  public:
    ObjectConsumer(String const & name, ObjectWriter * writer=0);
    ~ObjectConsumer();
    String const & getName() const     { return m_name; };
    void report(Object const * object)
    {
      if (m_writer != 0)
        m_writer->write(object);
      m_count++;
    };
    void finished();

  private:
    String             m_name;
    ObjectWriter *     m_writer;
    unsigned long long m_count;

    ObjectConsumer();
    ObjectConsumer(ObjectConsumer const &);
    ObjectConsumer & operator=(ObjectConsumer const &);
};

#endif // ifndef OBJECTCONSUMER_DEFINED
