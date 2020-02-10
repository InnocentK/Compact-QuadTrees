/*****************************************
 ** File:    QTree.cpp
 ** Project: CMSC 341 Project 3 - QuadTrees, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    04/10/18
 ** Section: 03
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of
 **   This class is

 ***********************************************/
#include "QTree.h"
#include <iostream>

// Default constructor
QTree::QTree(){

  m_root = NULL;
  m_size = 0;
}

//Quad Tree's destructor
QTree::~QTree(){

  // Checks if root exists before deleting it
  if (m_root != NULL)
    delete m_root;
  m_size = 0;
}

// Adds QTNodes to the quad tree
// Returns whether or not the defined node was added
bool QTree::add(const Point &pt, int data){

  bool added = false;

  // Edge case for adding first node
  if (m_size == 0){
    m_root = new QTNode(pt, data);
    added = true;
    m_size++;
}

  // Edge case for adding second node
  else if (m_size == 1) {
    QTNode* child1 = new QTNode(pt, data);
 
    // Checks if pointed added is a duplicate
    if (m_root->m_point == pt){
      m_root->m_data = data;
      delete child1;
      child1 = NULL;
      return added;
    }
 
    // Setting a new root
    QTNode* child2 = m_root;
    m_root = new QTNode();
    m_root->m_bounds.resize(child2->m_point,pt);

    // Checking if illegal bounding boxes
    if( (child1->m_bounds.inBounds(pt)) || (child2->m_bounds.inBounds(child1->m_point)) )
      child1->rescale(child2);
    else if( child1->m_bounds.overlaps(child2->m_bounds) )
      child1->rescale(child2);

    // Updating member variables
    m_root->m_child.at(0) = child2;
    m_root->m_num_children++;
    m_root->m_child.at(1) = child1;
    m_root->m_num_children++;
    added = true;
    m_size++;
  }

  // All other cases are handeled by QTNode add function
  // Ex. When size of the tree is >= 2
  else{
    const int MAX_CHILDREN = 4;

    // Root's bounds are too small
    if(m_root->m_bounds.m_dim < MAX_CHILDREN)
      m_root->m_bounds.grow();
    
    added = m_root->add(pt, data);
  }
  if (added)
    m_size++;
  return added;
}

// Removes QTNodes from the quad tree
// Returns whether or not the defined node was removed
bool QTree::remove(const Point &pt){

  return 9999;
}

// Finds QTNodes in the tree
// Returns whether or not the defind node was found
bool QTree::find(const Point &pt, int &data){

  bool found = false;
  const int MAX_CHILDREN = 4;

  // Tree has not been created
  if (m_root == NULL)
    found = false;

  // Point looking for is the root
  else if (m_root->m_point == pt){
    found = true;
    data = m_data;
  }

  // Point is not the root and root has no children
  else if (m_root->m_num_children < 1)
    found = false;

  // Point is likely within the bounds of the root
  else if (m_root->m_bounds.inBounds(pt)){

    // Loops thorugh children looking for point within non-null children
    for (int i = 0; i < MAX_CHILDREN; i++){
      if(m_root->m_child.at(i) != NULL)
	found = m_root->m_child.at(i)->find(pt, data);
    }
  }
  // If not within the bounds of the root the point does not exist
  return found;
}

// Reports all points found within a specified region
int QTree::findPoints(const BBox &region, std::vector<Point> &found){

  return 9999;
}

// Debugging function
void QTree::dump(){
  std::cout << "Printing the QuadTree:" << std::endl;

  // Error checking for empty tree
  if (m_root != NULL)
    m_root->dump();
  else 
    std::cout << "Empty Quad Tree";
  std::cout << std::endl;
}
