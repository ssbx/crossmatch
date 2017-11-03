/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * ALStructure is the main class in the implementation of the
 * data structure used by IndexedActiveList.  This data
 * structure is a collection of elements linked by two doubly
 * linked lists.  One of these lists is sorted by right
 * ascension, while the other is sorted by declination.  In
 * addition, a binary tree indexes the list on right ascension.
 * 
 * This data structure assumes that objects are added in
 * ascending order of declination.  If objects will not be added
 * in ascending order of declination, then do not use this data
 * structure: it won't work.
 */

#ifndef ALSTRUCTURE_DEFINED
#define ALSTRUCTURE_DEFINED

#include "ALElement.h"
#include "ALNode.h"


class ALStructure
{
  public:
    ALStructure();
    ~ALStructure();

    ALElement * getRaHead()  { return raHead; };
    ALElement * getDecHead() { return decHead; };
    ALElement * getTail()    { return tail; };

    void remove(ALElement * element);
    void add(ActiveObject * activeObject);

    /* find the element whose content is smaller while still
     * being greater than or equal to a given value.  Used
     * to find a starting object for walking the list.
     */
    ALElement * findLeastGE(double ra);

    /* find the element whose content is smaller while still
     * being greater than a given value.  Used
     * to find a stopping object for walking the list.
     */
    ALElement * findLeastG(double ra);

    unsigned int getSize();

  private:
    unsigned int size;
      
    ALElement sentinel;
    ALElement * raHead;
    ALElement * decHead;
    ALElement * tail;

    ALNode * rootNode;

    enum Child {left, right};

    ALStructure(ALStructure const &);
    ALStructure & operator=(ALStructure const &);

    void findNode(ALElement * element, ALNode * root, ALNode ** parent, Child * child);
    ALNode * grabSuccessor(ALNode * node);
    void removeNode(ALElement * element);
    ALElement * insertNew(ActiveObject * object, ALElement * nextRa, ALElement * nextDec);
    ALElement * findLowerBound(double ra, bool allowEquality);

};

#endif // ifndef ALSTRUCTURE_DEFINED
