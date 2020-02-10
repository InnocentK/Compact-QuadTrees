/*****************************************
 ** File:    QTNode.cpp
 ** Project: CMSC 341 Project 3 - QuadTrees, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    04/10/18
 ** Section: 03
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of 
 **   This class is 

 ***********************************************/
#include "QTNode.h"
#include <iostream>
//#define QT_NUM_KIDS 4

// QTNode Constructor
QTNode::QTNode(){

  unsigned int zero = 0;
  unsigned int default_dim = 1;
  m_child = new QTNode* [QT_NUM_KIDS];

  // Initialize all pointers to NULL
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;

  m_point = Point(zero,zero);
  m_bounds = BBox(m_point,default_dim);
  //m_bL = m_point;
  m_data = 0;
  m_num_children = 0;
}

// Overloaded Constructor
// default value of 0 for data
// For creating a new leaf node
QTNode::QTNode(const Point &pt, int data){

  unsigned int default_dim = 1;
  m_child = new QTNode* [QT_NUM_KIDS];

  // Initialize all pointers to NULL
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;

  m_point = pt;
  m_bounds = BBox(m_point,default_dim);
  //m_bL = m_point;
  m_data = data;
  m_num_children = 0;
}

// 2nd Overloaded Constructor
// default value of 0 for data
// For creating a new internal node
QTNode::QTNode(BBox bounds, int data){

  unsigned int zero = 0;
  m_child = new QTNode* [QT_NUM_KIDS];

  // Initialize all pointers to NULL
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;

  m_point = Point(zero,zero);
  m_bounds = bounds;
  //m_bL = m_bounds.m_bL;
  m_data = data;
  m_num_children = 0;
}

// 3rdd Overloaded Constructor
// default value of 0 for data
// For creating a new internal node
QTNode::QTNode(const Point &pt, BBox bounds, int data){

  m_child = new QTNode* [QT_NUM_KIDS];

  // Initialize all pointers to NULL
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child[i] = NULL;

  m_point = pt;
  m_bounds = bounds;
  //m_bL = m_bounds.m_bL;
  m_data = data;
  m_num_children = 0;
}

// Destructor
QTNode::~QTNode(){
  if (m_child != NULL){ 

    // Loops through children deleting those that exist
    for (int i = 0; i < QT_NUM_KIDS; i++){
      
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
  QTNode* new_child = new QTNode(pt, data);

  // Checks for dupilcates
  if (m_point == pt){
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
    return added;
  }

  // Quadrant adding to is occupied
  
  // Node is a leaf so it's data must be moved down
  if (m_child[quad]->m_num_children < 1){

    // Creating a new internal node to hold both points
    QTNode* new_leaf = m_child[quad];
    m_child[quad] = new QTNode(new_leaf->m_point, new_leaf->m_bounds);
    m_child[quad]->m_point = new_leaf->m_point;
    m_child[quad]->m_bounds.resize(new_leaf->m_point, pt);
    
    // Updating memebers
    m_child[quad]->m_child[0] = new_leaf;
    m_child[quad]->m_num_children++;
    m_child[quad]->m_child[4] = new_child;;
    m_child[quad]->m_num_children++;

    added = true;
    return added;

  }

  // Recursively add into quadrant
  // This is in case the point is lower in the quadrant
  //if( m_child[quad].m_bounds->inBounds(pt) ){
  added = m_child[quad]->add(pt, data);
  
  // Cleaning up unsued memory
  delete new_child;
  new_child = NULL;
  
  return added;
}

// removes a point
// returns whether or not that point was sucessfully removed
bool QTNode::remove(const Point &pt, bool &empty){

  bool removed = false;

  return 9999;
}

// finds a point
// returns whether or not the point was found
bool QTNode::find(const Point &pt, int &data){

  bool found = false;

  // Point looking for is the root                                                                  
  if (m_point == pt) {
    found = true;
    data = m_data;
  }

  // Point is not the root and root has no children                                                 
  else if (m_num_children < 1)
    found = false;

  // Point is likely within the bounds of the root                                                  
  else if (m_bounds.inBounds(pt)){

    // Loops thorugh children looking for point within non-null children
    for (int i = 0; i < QT_NUM_KIDS; i++){
      if(m_child[i] != NULL)
        found = m_child[i]->find(pt, data);
    }
  }
  // If not within the bounds of the root the point does not exist
  return found;
}

//
int QTNode::findPoints(const BBox &region, std::vector<Point> &found){

  return 9999;
}

// Debugging function
void QTNode::dump(){

  // Interior Nodes have default  data of 0
  if (m_data == 0)
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
  //temp++;
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
  return iterator(this,3);
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

// Helper functions to compare coordinates of the QTNodes
bool QTNode::largerX(QTNode* other){
  return m_point.m_x > other->m_point.m_x;
}
bool QTNode::largerY(QTNode* other){
  return m_point.m_y > other->m_point.m_y;
}
bool QTNode::smallerX(QTNode* other){
  return m_point.m_x < other->m_point.m_x;
}
bool QTNode::smallerY(QTNode* other){
  return m_point.m_y > other->m_point.m_y;
}
bool QTNode::sameX(QTNode* other){
  return m_point.m_x == other->m_point.m_x;
}
bool QTNode::sameY(QTNode* other){
  return m_point.m_y > other->m_point.m_y;
}
