/*****************************************
 ** File:    QTNode.cpp
 ** Project: CMSC 341 Project 3 - QuadTrees, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    04/10/18
 ** Section: 03
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of the QTNode class 
 **   This class is a node that has four children. The class
 **    also helps to manage operations done on these children
 **
 ***********************************************/
#include "QTNode.h"
#include <iostream>

// QTNode Constructor
QTNode::QTNode(){

  unsigned int default_dim = 1;
  m_child = new QTNode* [QT_NUM_KIDS + 1];

  // Initialize all pointers to NULL
  // Aside from dummy
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;
  m_child[QT_NUM_KIDS] = new QTNode(true);

  m_point = Point(0,0);
  m_bounds = BBox(m_point,default_dim);
  m_leaves = 0;
  m_data = 0;
  m_num_children = 0;
  m_dummy = false;
}

// Special Overloaded Constructor
// Used to define "dummy" nodes
// Dummy nodes are only used with the iterator
//  (they are the value for m_end in the iterator class)
QTNode::QTNode(bool dummy){

  unsigned int default_dim = 1;
  m_child = new QTNode* [QT_NUM_KIDS + 1];

  // Initialize all pointers to NULL
  for (int i = 0; i <= QT_NUM_KIDS; i++)
    m_child[i] = NULL;

  m_point = Point(0,0);
  m_bounds = BBox(m_point,default_dim);
  m_leaves = 0;
  m_data = 0;
  m_num_children = 0;
  m_dummy = dummy;
}

// Overloaded Constructor
// default value of 0 for data and leaves
// For creating a new leaf node
QTNode::QTNode(const Point &pt, int leaves, int data){

  unsigned int default_dim = 1;
  m_child = new QTNode* [QT_NUM_KIDS + 1];

  // Initialize all pointers to NULL
  // Aside from dummy
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;
  m_child[QT_NUM_KIDS] = new QTNode(true);

  m_point = pt;
  m_bounds = BBox(m_point,default_dim);
  m_leaves = leaves;
  m_data = data;
  m_num_children = 0;
  m_dummy = false;
}

// 2nd Overloaded Constructor
// default value of 0 for data and leaves
// For creating a new internal node
QTNode::QTNode(BBox bounds, int leaves, int data){

  m_child = new QTNode* [QT_NUM_KIDS + 1];

  // Initialize all pointers to NULL
  // Aside from dummy
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;
  m_child[QT_NUM_KIDS] = new QTNode(true);

  m_point = Point(0,0);
  m_bounds = bounds;
  m_leaves = leaves;
  m_data = data;
  m_num_children = 0;
  m_dummy = false;
}

// 3rd Overloaded Constructor
// default value of 0 for data and leaves
// For creating a new internal node with a fixed point
QTNode::QTNode(const Point &pt, BBox bounds, int leaves, int data){

  m_child = new QTNode* [QT_NUM_KIDS + 1];

  // Initialize all pointers to NULL
  // Aside from dummy
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;
  m_child[QT_NUM_KIDS] = new QTNode(true);

  m_point = pt;
  m_bounds = bounds;
  m_leaves = leaves;
  m_data = data;
  m_num_children = 0;
  m_dummy = false;
}

// Destructor
QTNode::~QTNode(){
  if (m_child != NULL){ 

    // Loops through children deleting those that exist
    // Including dummy
    for (int i = 0; i <= QT_NUM_KIDS; i++){
      
      // Checks if child exists
      if (m_child[i] != NULL)
	delete m_child[i];
    }
    delete [] m_child;
    m_child = NULL;
  }
}

// adds a new point
// returns whether or not that point was successfully added
// Assumes the point is already initilised
// Unitialized points are handeled by QTree::add() or through recusrive
//  calls of add
bool QTNode::add(const Point &pt, int data){

  bool added = false;
  int leaf = 1;
  QTNode* new_child = new QTNode(pt, leaf, data);

  // Checks for dupilcates
  // Does not include internal nodes
  if( (m_point == pt) && (m_num_children < 1) ){
    m_data = data;
    delete new_child;
    new_child = NULL;
    return added;
  }

  int quad = getQuad(pt);

  // Adding to an empty node
  if (m_child[quad] == NULL){
    m_child[quad] = new_child;
    m_num_children++;
    added = true;
    m_leaves++;
    return added;
  }

  // Quadrant adding to is occupied
  
  // Node is a leaf so it's data must be moved down
  if (m_child[quad]->m_num_children < 1){

    // Creating a new internal node to hold both points
    QTNode* new_leaf = m_child[quad];
    m_child[quad] = new QTNode(new_leaf->m_point, new_leaf->m_bounds, new_leaf->m_leaves);
    m_child[quad]->m_point = new_leaf->m_point;
    m_child[quad]->m_bounds.resize(new_leaf->m_point, pt);

    // Updating memebers
    m_child[quad]->m_child[0] = new_leaf;
    m_child[quad]->m_num_children++;
    m_child[quad]->m_child[4] = new_child;
    m_child[quad]->m_num_children++;

    added = true;
    m_child[quad]->m_leaves++;    
    return added;

  }

  // Recursively add into quadrant
  // This is in case the point is lower in the quadrant
  added = m_child[quad]->add(pt, data);
  
  // Cleaning up unsued memory
  delete new_child;
  new_child = NULL;
  
  return added;
}

// removes a point
// removal always happens from a parent, deleting a child
// returns whether or not that point was sucessfully removed
bool QTNode::remove(const Point &pt, bool &empty){


  bool removed = false;

  // Point is likely within the bounds of current node
  if (m_bounds.inBounds(pt)){
    int quad = getQuad(pt);

    // Base Case, when area where point should be is empty
    if (m_child[quad] == NULL)
      removed = false;

    // Child Node is the one needing removal
    else if( (m_child[quad]->m_point == pt) &&
             (m_child[quad]->m_num_children < 1) ){
   
      delete m_child[quad];
      m_child[quad] = NULL;
      m_num_children--;
      m_leaves--;
      removed = true;
      
      // Node is no longer needed
      if (m_leaves < 1)
	empty = true;
      
      // Recusrively deletes nodes after a leaf has been removed
      // Recursion occurs because this is called after a return from
      //  a previously created recursion but before the final return
      if( (m_child[quad] != NULL) && (empty) ){
	delete m_child[quad];
	m_child[quad] == NULL;
	m_num_children--;
      }

      // Root cannot be left with only one child 
      if (m_num_children == 1) {

        // Remaining child becomes new root
        for (int i; i < QT_NUM_KIDS; i++){

	  // Copying members from child and deleting child node
	  // Basically promoting child
          if (m_child[i] != NULL){
            m_point = m_child[i]->m_point;
	    m_bounds = m_child[i]->m_bounds;
	    m_data = m_child[i]->m_data;
	    delete m_child[i];
            m_child[i] = NULL;
	    m_num_children--;
	    return removed;
          }
        }
      }      
    }
    
    // Recursive case, when need to look thorugh next child for point
    else
      removed = m_child[quad]->remove(pt,empty);
  }

  if (removed)
    m_leaves--;

  // If not within the bounds of the root the point does not exist
  return removed;

}

// finds a point
// returns whether or not the point was found
bool QTNode::find(const Point &pt, int &data){

  bool found = false;

  // Point looking for is the root                                                                  
  if( (m_point == pt) && (m_data != 0) ){
    found = true;
    data = m_data;
  }

  // Point is not the root and root has no children                                                 
  else if (m_num_children < 1)
    found = false;

  // Point is likely within the bounds of the root                                                  
  else if (m_bounds.inBounds(pt)){

    int quad = getQuad(pt);

    // Base Case, when area where point should be is empty
    if (m_child[quad] == NULL)
      found = false;

    // Recursive case, when need to look thorugh next child for point
    else
      found = m_child[quad]->find(pt, data);
  }
  // If not within the bounds of the root the point does not exist
  return found;
}

// finds all points within a specified region
// reports those points (the number of points and appends the actual
//   points to a given vector)
int QTNode::findPoints(const BBox &region, std::vector<Point> &found){
  int num_points = 0;

  // Region looking for is the current node
  if (m_bounds.m_bL == region.m_bL){

    // When node is a leaf
    if (m_num_children < 1){
      found.push_back(m_point);
      num_points++;
    }

    // If node a leaf then add up all the points within its subtree
    else{
      const int MAX_CHILDREN = 4;

      // Appends all leaves to found vector
      for (int i = 0; i < MAX_CHILDREN; i++)
	if (m_child[i] != NULL)
	  num_points += m_child[i]->findPoints(m_child[i]->m_bounds,found);
    }
  }

  // Region searching for is within the current subtree
  else if( (m_bounds.inBounds(region.m_bL)) || 
	   (region.inBounds(m_bounds.m_bL)) ){

    int quad = getQuad(region.m_bL);

    // Base Case, when area looking for is empty
    if (m_child[quad] == NULL)
      return num_points;

    // Recursive case, when need to look thorugh next child for point
    else
      num_points += m_child[quad]->findPoints(region,found);
  }
  return num_points;
}

// Overloaded findPoints that takes in 2 extra arguments
// Extra arguments: the maximum and minimum point of the region searching   
int QTNode::findPoints(const BBox &region, std::vector<Point> &found, const Point &max, const Point &min){
  int num_points = 0;

  // Region looking for is the current node
  if (m_bounds.m_bL == region.m_bL){

    // Checks if point is truley within the desired region
    bool in_region = ( (m_point.m_x <= max.m_x) && (m_point.m_y <= max.m_y) && 
		       (m_point.m_x >= min.m_x) && (m_point.m_y >= min.m_y) );
    
    // When node is a leaf
    if( (m_num_children < 1) && (in_region) ){
      found.push_back(m_point);
      num_points++;
    }

    // If node a leaf then add up all the points within its subtree
    else{
      const int MAX_CHILDREN = 4;

      // Appends all leaves to found vector
      for (int i = 0; i < MAX_CHILDREN; i++)
	if (m_child[i] != NULL)
	  num_points += m_child[i]->findPoints(m_child[i]->m_bounds,found, max, min);
    }
  }

  // Region searching for is within the current subtree
  else if( (m_bounds.inBounds(region.m_bL)) || 
	   (region.inBounds(m_bounds.m_bL)) ){

    int quad = getQuad(region.m_bL);

    // Base Case, when area looking for is empty
    if (m_child[quad] == NULL)
      return num_points;

    // Recursive case, when need to look thorugh next child for point
    else
      num_points += m_child[quad]->findPoints(region,found, max, min);
  }
  return num_points;
}

// Debugging function
void QTNode::dump(){

  // Leaves have no children
  if (m_num_children > 0)
    std::cout << "[QTNode bounds=" << m_bounds << " is INTERNAL:" << std::endl;
  else{
    std::cout << "[QTNode bounds=" << m_bounds << " is LEAF: pt=" << m_point 
	      << ", data=" << m_data << "]" << std::endl;
    return;
  }

  // Loop is recursive
  for (int i = 0; i < QT_NUM_KIDS; i++){

    // Tells what child is bring printed
    switch(i){
    case 0:
      std::cout << "Botom-Left child:" << std::endl;
      break;
    case 1:
      std::cout << "Botom-Right child:" << std::endl;
      break;
    case 2:
      std::cout << "Top-Left child:" << std::endl;
      break;
    default:
      std::cout << "Top-Right child:" << std::endl;
    }
    
    // Recursively prints children
    if (m_child[i] != NULL)
      m_child[i]->dump();
    else
      std::cout << "[NULL]" << std::endl;
  }
  std::cout << "]"  << std::endl;
}

// Iterator and it's necessary methods
// Default constructor
QTNode::iterator::iterator(){
  m_pos = 0;
}

// Overloaded constructor
// pos has a default value of 0
QTNode::iterator::iterator(QTNode* node, int pos){
  m_pos = pos;
  parent = node;
  child = parent->m_child[m_pos];
}

// Checks if two iterators are equal
bool QTNode::iterator::operator==(const QTNode::iterator &other){
  return child == other.child;
}

// Checks inequality from iterators
bool QTNode::iterator::operator!=(const QTNode::iterator &other){
  return child != other.child;
}

// Prefix: e.g. "++it"
QTNode::iterator& QTNode::iterator::operator++(){
  child = parent->m_child[++m_pos];
  return *this;
}          

// Postfix: "it++"
QTNode::iterator QTNode::iterator::operator++(int dummy){
  QTNode::iterator temp(parent);
  operator++();
  return *temp;
}  

// Derefernces an iterator
// Because iterator is applied on parent QTNode the dereference returns the child
//  since the children are the ones being manipulated and iterated through
QTNode*& QTNode::iterator::operator*(){
  return parent->m_child[m_pos];
}

// Returns iterator pointing to the first child
QTNode::iterator QTNode::begin(){
  return iterator(this);
}

// Returns iterator pointing to the last child
QTNode::iterator QTNode::end(){
  return iterator(this,4);
}

// Additional Functions to help with QTNode operations

// Calculates which quadrant a point should reside in current QTNode
int QTNode::getQuad(const Point &pt){

  Point mid = getMid();
  enum {BL, BR, TL, TR, OVERLAP};

  // Top-Right
  if( (mid.m_x <= pt.m_x) && (mid.m_y <= pt.m_y) )
    return TR;

  // Top-Left
  else if( (mid.m_x > pt.m_x) && (mid.m_y <= pt.m_y) )
    return TL;

  // Bottom-Right
  else if( (mid.m_x <= pt.m_x) && (mid.m_y > pt.m_y) )
    return BR;

  // Bottom-Left
  else if ( (mid.m_x > pt.m_x) && (mid.m_x > pt.m_x) )
    return BL;

  // Only called when both points have the same X and Y
   return OVERLAP;
}

// Calculates the center of a bounds
// Returns the center as a point
Point QTNode::getMid(){
  unsigned int mid_x = m_point.m_x + (m_bounds.m_dim/2);
  unsigned int mid_y = m_point.m_y + (m_bounds.m_dim/2);
  Point mid(mid_x, mid_y);
  return mid;
}

// Calculates the maximum point a bounds can have
Point QTNode::getMax(){
  unsigned int max_x = m_point.m_x + m_bounds.m_dim;
  unsigned int max_y = m_point.m_y + m_bounds.m_dim;
  Point max(max_x, max_y);
  return max;
}
