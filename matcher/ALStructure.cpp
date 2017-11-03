/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

/* ALStructure is the main class in the implementation of the
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
 *
 * Consequently, every insertion and deletion involves three
 * separate operations.  The binary tree operations tend to
 * be a tad complex, so if you intend to edit this file, I
 * suggest that you familiarise yourself with binary trees
 * first.
 *
 * The only other detail worth mentioning is this: in order
 * to simplify the specification of insertion objects, it
 * was convenient to implement this class with a sentinel
 * element.  That is, a dud element that contains no data
 * but merely indicates the end of a list.  Because of the
 * presence of this sentinel element, the lists should never
 * be completely empty.  Also, because of the sentinal element,
 * the tails of the list necessarily coincide at all times;
 * hence there are two head objects: raHead and decHead, but
 * only a single tail object: tail.
 */
#include <iostream>

#include "ALStructure.h"


ALStructure::ALStructure()
    : size(0),
      sentinel(0, 0, 0, 0, 0),
      raHead(&sentinel),
      decHead(&sentinel),
      tail(&sentinel),
      rootNode(0)
{
  rootNode = new ALNode(&sentinel, 0, 0);
}

ALStructure::~ALStructure()
{
  delete rootNode;
}

/* find the element whose active object has the smallest ra
 * greater than or equal to ra */
ALElement * ALStructure::findLeastGE(double ra)
{
  return findLowerBound(ra, true);
}

/* find the element whose active object has the smallest ra
 * greater than ra */
ALElement * ALStructure::findLeastG(double ra)
{
  return findLowerBound(ra, false);
}

/* find the element whose active object has the smallest ra
 * greater than ra.  The boolean flag indicates whether
 * equality is permitted. */

ALElement * ALStructure::findLowerBound(double ra, bool allowEquality)
{
  /* Start at the root */
  ALNode * currentNode = rootNode;

  /* Keep track of the best element found so far. */
  ALElement * bestSoFar = 0;

  /* Until we have walked all the way down to a leaf... */
  while (currentNode != 0)
  {
    ALElement * i = currentNode->getElement();

    if (i == tail)
    {
      // this is the sentinel node, which is by definition greater than c.
      // This is the best lower bound so far. 
      // Take a note of it, and look to the left for something smaller still.
      bestSoFar = i;
      currentNode = currentNode->getLeftChild();
    }
    else
    {
      // this is a normal node.  Check its value.
      ActiveObject * content = i->getContent();
      double contentRa = content->getObject()->getRA();
      if (contentRa > ra)
      {
        // This is greater than the bound, as required.
        // Since we always step to the left once a
        // suitable element has been found, this must
        // be the best so far. Take a note of it, and
        // look to the left for something smaller still.
        bestSoFar = i;
        currentNode = currentNode->getLeftChild();
      }
      else if (contentRa < ra)
      {
        // Oops, this is less than the bound.  We have
        // searched too far to the left.  Look right.
	currentNode = currentNode->getRightChild();
      }
      else if (allowEquality)
      {
        // We have found an element equal to the bound,
        // and this is permitted.  Since we always step
        // to the left when a suitable element has been
        // found, this must be the best so far.
        // Take a note of it, and look to the left.
        // We continue searching because if there are
        // many objects with the same ra, we want to find
        // the leftmost one.
        bestSoFar = i;
        currentNode = currentNode->getLeftChild();
      }
      else
      {
        // Oops, we have found an element equal to the
        // bound, and this is not permitted.  We have
        // searched too far to the left.  Look right.
        currentNode = currentNode->getRightChild();
      }
    }
  }

  // bestSoFar now contains the smallest element meeting
  // the provided condition.  If there is no such element,
  // the sentinel will be returned.
  return bestSoFar;
}


/* remove an element from the data structure. */
void ALStructure::remove(ALElement * element)
{
  ALElement * prevRa = element->getPrevRa();
  ALElement * nextRa = element->getNextRa();
  ALElement * prevDec = element->getPrevDec();
  ALElement * nextDec = element->getNextDec();

  // update the lists.

  // element is guaranteed to have a next, because it is not the
  // sentinel (presumably).  But there's no guarantee that prevRa
  // or prevDec aren't 0, since element might be at the head of
  // one or both lists.  We therefore need to be careful when
  // removing elements.
 
  nextRa->setPrevRa(prevRa);
  if (prevRa == 0)
    raHead = nextRa;
  else
    prevRa->setNextRa(nextRa);

  nextDec->setPrevDec(prevDec);

  if (prevDec == 0)
    decHead = nextDec;
  else
    prevDec->setNextDec(nextDec);

  // now remove the node from the index.
  removeNode(element);

  delete element;

  size--;
}

void ALStructure::removeNode(ALElement * element)
{
  ALNode * target;

  if (rootNode->getElement() == element)
  {

// this stuff commented out because its only use is
// for deletion of the sentinel element.  This code
// does actually work, but if a calling function
// demands deletion of the sentinel, I'd rather
// fail fast, than robustly delete the sentinel
// then fail later in some strange way.
//
//    target = rootNode;
//    if ((rootNode->getLeftChild() == 0)
//    && (rootNode->getRightChild() == 0)) {
//      rootNode = 0;
//    } else if (rootNode->getRightChild() == 0) {
//      rootNode = rootNode->getLeftChild();
//    } else if (rootNode->getLeftChild() == 0) {
//      rootNode = rootNode->getRightChild();
//    } else {
//      ALNode * successor = grabSuccessor(target);
//      
//      rootNode = successor;
//    }

    std::cout << "Error: trying to delete the sentinel!\n";
  }
  else
  {
    ALNode * parent;
    Child child;

    // find the node to be deleted.  Return the result
    // as a objecter to the node's parent, and an
    // indication of which child the node is.
    findNode(element, rootNode, &parent, &child);

    // make target object to the node to be removed.
    if (child == left)
      target = parent->getLeftChild();
    else
      target = parent->getRightChild();
    
    if ((target->getLeftChild() == 0) &&
        (target->getRightChild() == 0))
    {
      // target has no children, so simply delete
      if (child == left)
        parent->setLeftChild(0);
      else
        parent->setRightChild(0);
    }
    else if (target->getRightChild() == 0)
    {
      // target has no right subtree, so replace with left subtree
      if (child == left)
        parent->setLeftChild(target->getLeftChild());
      else
        parent->setRightChild(target->getLeftChild());
    }
    else if (target->getLeftChild() == 0)
    {
      // target has no left subtree, so replace with right subtree
      if (child == left)
        parent->setLeftChild(target->getRightChild());
      else
        parent->setRightChild(target->getRightChild());
    }
    else
    {
      // target has two children, so replace with inorder successor
      ALNode * successor = grabSuccessor(target);

      // connect parent to successor.
      if (child == left)
        parent->setLeftChild(successor);
      else
        parent->setRightChild(successor);

      // connect successor to target's left child
      successor->setLeftChild(target->getLeftChild());
    }
    delete target;
  }
}

/* Find the node containing a given element, and return
 * the answer as the parent of the node, and an indication
 * of which child the node is.  This is safe because we'll
 * never go looking for the root node, since this is the
 * sentinel. */
void ALStructure::findNode(ALElement * element,
                           ALNode * root,
                           ALNode * * parent,
                           ALStructure::Child * child)
{
  // at entry, element != root->getElement, so there's no
  // need to check
  ActiveObject * content = element->getContent();
  ActiveObject * rootContent = root->getElement()->getContent();

  // compare the content of the element to be found with the
  // content of the current root.  This will tell us whether
  // to search left, right, or both.
  int comp;
  if (rootContent == 0)
    comp = -1;
  else if (content->getObject()->getRA() < rootContent->getObject()->getRA())
    comp = -1;
  else if (content->getObject()->getRA() > rootContent->getObject()->getRA())
    comp = 1;
  else
    comp = 0;

  if (comp <= 0)
  {
    // we need to search to the left.
    ALNode * leftChild = root->getLeftChild();
    if (leftChild != 0)
    {
      // there is a left child, so check if it is the
      // element we're looking for.
      if (leftChild->getElement() == element)
      {
        // found it!  return the results.
        *parent = root;
        *child = left;
        return;
      }
      // there's a left child, but it isn't the
      // one we're looking for.  Recursively search
      // for the node in this subtree.
      findNode(element, leftChild, parent, child);

      // our recursive search succeeded.
      // Don't search any further.
      if (*parent != 0)
        return;
    }
  }

  if (comp >= 0)
  {
    // we need to search to the right.
    ALNode * rightChild = root->getRightChild();
    if (rightChild != 0)
    {
      // there is a right child, so check if it is the
      // element we're looking for.
      if (rightChild->getElement() == element)
      {
        // found it!  return the results.
        *parent = root;
        *child = right;
        return;
      }
      // there's a right child, but it isn't the
      // one we're looking for.  Recursively search
      // for the node in this subtree.
      findNode(element, rightChild, parent, child);

      // our recursive search succeeded.
      // Don't search any further.
      if (*parent != 0)
        return;
    }
  }

  // our search failed.  This subtree does not contain
  // the node we are looking for.  Return null.
  *parent = 0;

  return;
}

/* Find the inorder successor of a given node. */
ALNode * ALStructure::grabSuccessor(ALNode * delNode)
{
  ALNode * successorParent = delNode;
  ALNode * successor = delNode;
  ALNode * current = delNode->getRightChild();

  while (current != 0)
  {
    successorParent = successor;
    successor = current;
    current = current->getLeftChild();
  }

  if (successor != delNode->getRightChild())
  {
    successorParent->setLeftChild(successor->getRightChild());
    successor->setRightChild(delNode->getRightChild());
  }

  return successor;
}


/* Add a object to the data structure.  This involves
 * three steps.  First we append it to the dec list --
 * this is always correct because objects are presented
 * in dec order.  Second we insert it into the ra list.
 * And finally we update the ra index. */
void ALStructure::add(ActiveObject * object)
{
  ALNode * currentNode = rootNode;
  ALNode * parentNode;

  double ra = object->getObject()->getRA();

  /* search the ra index for an insertion object. */
  while (true)
  {
    parentNode = currentNode;
    ALElement * element = currentNode->getElement();
    if (element == tail)
    {
      // this is the sentinel node, which is by definition
      // greater than c.  So search to the left.
      currentNode = currentNode->getLeftChild();
      if (currentNode == 0)
      {
        // we're at a leaf.  Insert an element into
        // the lists here...
        ALElement * newElement = insertNew(object, parentNode->getElement(), tail);

        // ... then update the index.
        ALNode * newNode = new ALNode(newElement, 0, 0);
        parentNode->setLeftChild(newNode);
        return;
      }
    }
    else
    {
      // this is a normal element.  Check the content to
      // determine whether to insert our new element
      // before or after this element.
      ActiveObject * current = element->getContent();
      if (ra < current->getObject()->getRA())
      {
        // we'll need to insert before.  Look to the left.
        currentNode = currentNode->getLeftChild();
        if (currentNode == 0)
        {
          // we're at a leaf, insert an element into
          // the lists here...
          ALElement * newElement = insertNew(object, parentNode->getElement(), tail);

          // ... then update the index.
          ALNode * newNode = new ALNode(newElement, 0, 0);
          parentNode->setLeftChild(newNode);
          return;
        }
      }
      else
      {
        // we'll need to insert after.  Look to the right.
        currentNode = currentNode->getRightChild();
        if (currentNode == 0)
        {
          // we're at a leaf.  Insert an element into
          // the lists here...
          ALElement * newElement = insertNew(object, parentNode->getElement()->getNextRa(), tail);

          // ... then update the index.
          ALNode * newNode = new ALNode(newElement, 0, 0);
          parentNode->setRightChild(newNode);
          return;
        }
      }
    }
  }
}

/* This method just handles the list insertion component of adding
 * a new element to the data structure.  nextRa and nextDec are
 * guaranteed to be non-null, because of the existence of the
 * sentinel, but there is no guarantee that prevRa and prevDec are
 * non-null.  So we have to be careful */
ALElement * ALStructure::insertNew(ActiveObject * object,
                                   ALElement * nextRa,
                                   ALElement * nextDec)
{
  // locate the previous elements.
  ALElement * prevRa = nextRa->getPrevRa();
  ALElement * prevDec = nextDec->getPrevDec();

  // create a new element.
  ALElement * element = new ALElement(object, prevRa, nextRa, prevDec, nextDec);

  // correct the next elements' previous objecters.
  nextRa->setPrevRa(element);
  nextDec->setPrevDec(element);

  // if we are inserting at the head, then there is no
  // previous element needing to be updated, but we do
  // have to update the head objecters
  if (prevRa == 0)
  {
    raHead = element;
  }
  else      // otherwise, correct the previous elements' next objecters.
  {
    prevRa->setNextRa(element);
  }

  // if we are inserting at the head, then there is no
  // previous element needing to be updated, but we do
  // have to update the head objecters
  if (prevDec == 0)
  {
    decHead = element;
  }
  else
  {         // otherwise, correct the previous elements' next pointers.
    prevDec->setNextDec(element);
  }

  size++;

  return element;
}

unsigned int ALStructure::getSize()
{
  return size;
}
