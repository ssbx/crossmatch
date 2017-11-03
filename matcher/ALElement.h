/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * ALElement is a container for an entity to be stored on an
 * IndexedActiveList.  I had intended to use templates
 * to make it a generic container, but right now it is specific
 * to ActiveObjects.
 *
 * ALElement is intended to sit in two double linked lists, one
 * sorted by right ascension, and the other sorted by declination.
 */

#ifndef ALELEMENT_DEFINED
#define ALELEMENT_DEFINED

#include "ActiveObject.h"


class ALElement
{
  public:
    ALElement(ActiveObject * object,
              ALElement * pRa,
              ALElement * nRa,
              ALElement * pDec,
              ALElement * nDec)
        : content(object), prevRa(pRa), nextRa(nRa), prevDec(pDec), nextDec(nDec) {}
    ~ALElement() {};
    ActiveObject * getContent() const { return content; }
    ALElement *    getPrevRa() const  { return prevRa; }
    ALElement *    getNextRa() const  { return nextRa; }
    ALElement *    getPrevDec() const { return prevDec; }
    ALElement *    getNextDec() const { return nextDec; }

    void setPrevRa(ALElement * prev)  { prevRa = prev; }
    void setNextRa(ALElement * next)  { nextRa = next; }
    void setPrevDec(ALElement * prev) { prevDec = prev; }
    void setNextDec(ALElement * next) { nextDec = next; }

  private:
    ActiveObject * content;
    ALElement *    prevRa;
    ALElement *    nextRa;
    ALElement *    prevDec;
    ALElement *    nextDec;

    ALElement();
    ALElement(ALElement const &);
    ALElement & operator=(ALElement const &);
};

#endif // ifndef ALELEMENT_DEFINED
