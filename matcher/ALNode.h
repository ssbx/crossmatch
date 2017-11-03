/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/*
 * ALNode forms part of the indexing structure used by
 * IndexedActiveList.  It is basically a node in a
 * binary tree.  I had intended to code this up generically
 * using templates, but right now it is specific to
 * ALElements.
 */

#ifndef ALNODE_DEFINED
#define ALNODE_DEFINED

#include "ALElement.h"


class ALNode
{
  public:
    ALNode(ALElement * e,
           ALNode * lC,
           ALNode * rC)
        : element(e), leftChild(lC), rightChild(rC) {};
    ~ALNode() {};

    ALElement * getElement()    const { return element; };
    ALNode *    getLeftChild()  const { return leftChild; };
    ALNode *    getRightChild() const { return rightChild; };

    void setElement(ALElement * e)  { element = e; };
    void setLeftChild(ALNode * lC)  { leftChild = lC; };
    void setRightChild(ALNode * rC) { rightChild = rC; };

  private:
    ALElement * element;
    ALNode * leftChild;
    ALNode * rightChild;

    ALNode();
    ALNode(ALNode const &);
    ALNode & operator=(ALNode const &);
};


#endif // ifndef ALNODE_DEFINED
