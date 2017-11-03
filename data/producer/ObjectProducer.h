/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * A producer of objects. This class uses an ObjectReader
 * to create objects uing the hasNext and next methods.
 *
 * The different constructors are needed for the cases
 * where the max SD's are known for the source, when
 * they should be read from the source sequentially
 * (and the source reopenmed afterwards), or if they are
 * not needed.
 *
 * This class should not be subclassed. Different sources
 * are accomodated through the use of different ObjectReaders.
 *
 * The ObjectReader is "owned" by this class and will be 
 * deleted by the destructor.
 */

#ifndef OBJECTPRODUCER_DEFINED
#define OBJECTPRODUCER_DEFINED

class Object;
class ObjectReader;


class ObjectProducer
{
  public:
    /* Use this constructor when the maximum standard
     * deviations in declination and orthogonal to
     * declination are not known. If the setMaxSDs flag
     * is true, this ObjectProducer will scan the file 
     * for maximum standard devation values.
     */
    ObjectProducer(ObjectReader * reader,
                   bool setMaxSDs = false);

    /* Used when only one max SD is needed. */
    ObjectProducer(ObjectReader * reader,
                   double maxSD);

    /* Used when two max SD values are needed */
    ObjectProducer(ObjectReader * reader,
                   double maxOrthoSD,
                   double maxDecSD);
    
    ~ObjectProducer();
    bool hasNext() const;
    Object * next() const;

    double getMaxDecSD()   const { return maxDecSD; };
    double getMaxOrthoSD() const { return maxOrthoSD; };
    double getMaxSD()      const
    {
      return (getMaxDecSD() > getMaxOrthoSD()) ? getMaxDecSD() : getMaxOrthoSD();
    }

  private:
    ObjectReader *   reader;
    mutable Object * current;
    mutable bool     hasMore;
    mutable bool     isCurrent;
    mutable double   maxOrthoSD;
    mutable double   maxDecSD;

    ObjectProducer();
    ObjectProducer(ObjectProducer const &);
    ObjectProducer & operator=(ObjectProducer const &);

    void scanForMaxSDs() const;
};

#endif // ifndef OBJECTPRODUCER_DEFINED
