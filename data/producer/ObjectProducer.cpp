/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#include "ObjectProducer.h"
#include "ObjectReader.h"


ObjectProducer::ObjectProducer(ObjectReader * r, bool setMaxSDs)
    : reader(r),
      hasMore(false),
      isCurrent(false),
      maxOrthoSD(0.0),
      maxDecSD(0.0)
{
  reader->open();

  if (setMaxSDs)
  {
    scanForMaxSDs();
  }
}

ObjectProducer::ObjectProducer(ObjectReader * r, double maxSD)
    : reader(r),
      hasMore(false),
      isCurrent(false),
      maxOrthoSD(maxSD),
      maxDecSD(maxSD)
{
  reader->open();
}

ObjectProducer::ObjectProducer(ObjectReader * r, double mOSD, double mDSD)
    : reader(r),
      hasMore(false),
      isCurrent(false),
      maxOrthoSD(mOSD),
      maxDecSD(mDSD)
{
  reader->open();
}

ObjectProducer::~ObjectProducer()
{
  reader->close();
  delete reader;
}

void ObjectProducer::scanForMaxSDs() const
{
  std::cout << "Scanning for max SDs!" << std::endl;
  maxDecSD = 0.0;
  maxOrthoSD = 0.0;

  unsigned long long cnt = 0;
  while (hasNext())
  {
    cnt++;
    Object * obj = next();

    if (obj->getDecSD() > maxDecSD)
      maxDecSD = obj->getDecSD();
    if (obj->getOrthoSD() > maxOrthoSD)
      maxOrthoSD = obj->getOrthoSD();

    delete obj;
  }

  std::cout << "Num records: " << cnt << std::endl;

  reader->reopen();
}

bool ObjectProducer::hasNext() const
{
  if (isCurrent)
    return hasMore; // no next() call since last hasNext()

  current = reader->read();         // read the next object

  bool success = (current != 0);    // check if successfull
  hasMore = success;
  isCurrent = success;

  return success;
}
  
Object * ObjectProducer::next() const
{
  bool ok = true;

  if (!isCurrent)
  {                         // either hasNext() hasn't been called, or it
                            // returned false.
    if (!hasNext())         // if true, then isCurrent set true as side-effect
    {
      ok = false;
    }
  }
    
  // isCurrent is true now.
  if (ok)
  {
    isCurrent = false; // don't use the current data again
  }

  return current;
}
