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
  unsigned int default_dim = 4;

  // Initialize all pointers to NULL
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child.push_back(NULL);

  m_point = Point(zero,zero);
  m_bounds = BBox(m_point,default_dim);
  m_data = 0;
  m_num_children = 0;
}

// Overloaded Constructor
// default value of 0 for data
QTNode::QTNode(const Point &pt, int data){

  unsigned int zero = 0;
  unsigned int default_dim = 1;

  // Initialize all pointers to NULL
  for (int i = 0; i < QT_NUM_KIDS; i++)
    m_child.push_back(NULL);

  m_point = pt;
  m_bounds = BBox(m_point,default_dim);
  m_data = data;
  m_num_children = 0;
}

// Overloaded constructor
// Used to create a QTNode using all of a current node's members
QTNode::QTNode(const Point &pt, BBox bounds, int data, int num_children, std::vector<QTNode*>& child){

  m_point = pt;
  m_bounds = bounds;
  m_data = data;
  m_num_children = num_children;
  m_child = child;
}

// Destructor
QTNode::~QTNode(){

  // Loops through children deleting those that exist
  for (int i = 0; i < QT_NUM_KIDS; i++){

    // Checks if child exists
    if (m_child.at(i) != NULL){
      delete m_child.at(i);
      m_child.at(i) = NULL;
    }
  }
}

// adds a new point
// returns whether or not that point was successfully added
bool QTNode::add(const Point &pt, int data){

  bool added = false;
  QTNode* newChild = new QTNode(pt, data);

  // Checks for dupilcates
  if (m_point == pt){
    m_data = data;
    delete newChild;
    newChild = NULL;
    return added;
  }

  // Checking for leaf
  if( m_num_children < 1 ){

    // Creating a new parent
    QTNode* oldParent = new QTNode(m_point, m_bounds, m_data, m_num_children, m_child);

    // Clearing data in new parent
    for (int i = 0; i < QT_NUM_KIDS; i++)
      m_child.at(i) = NULL;
    m_data = 0;
    m_num_children = 0;
    m_bounds.resize(oldParent->m_point, pt);
    //m_bounds.grow();

    // Checking if illegal bounding boxes                                                            
    if( (oldParent->m_bounds.inBounds(pt)) || (newChild->m_bounds.inBounds(oldParent->m_point)) )
      oldParent->rescale(newChild);
    else if( oldParent->m_bounds.overlaps(newChild->m_bounds) )
      oldParent->rescale(newChild);

    // Updating member variables                                                                     
    m_child.at(0) = newChild;
    m_num_children++;
    m_child.at(1) = oldParent;
    m_num_children++;
    added = true;
    return added;
  }

  // Point belongs in current QTNode's bounds
  else if( m_bounds.inBounds(pt) ){
      
      // Looping through all children
      for (int i = 0; i < QT_NUM_KIDS; i++){
	
	// Checks for location to add child
	if (m_child.at(i) == NULL){
	  m_child.at(i) = newChild;
	  m_num_children++;
	  added = true;
	  return added;
	}
	
	// Checks if point should go within a child
	// Recursive
	else if( m_child.at(i)->m_bounds.inBounds(pt) )
	  added = m_child.at(i)->add(pt, data);
      }
    }
    
  // All other options have been exhausted
  // New node must be created to hold current and new
  else{

    // QTNode is full
    // Must create a new parent
    if (m_num_children == QT_NUM_KIDS){

      // Creating a new parent
      QTNode* oldParent = new QTNode(m_point, m_bounds, m_data, m_num_children, m_child);
      
      // Clearing data in new parent
      for (int i = 0; i < QT_NUM_KIDS; i++)
	m_child.at(i) = NULL;
      m_data = 0;
      m_num_children = 0;
      m_bounds.resize(oldParent->m_point, pt);
      m_bounds.grow();
      
      // Checking if illegal bounding boxes                                                            
      if( (oldParent->m_bounds.inBounds(pt)) || (newChild->m_bounds.inBounds(oldParent->m_point)) )
	oldParent->rescale(newChild);
      else if( oldParent->m_bounds.overlaps(newChild->m_bounds) )
      oldParent->rescale(newChild);
      
      // Updating member variables                                                                     
      m_child.at(0) = newChild;
      m_num_children++;
      m_child.at(1) = oldParent;
      m_num_children++;
      added = true;
      return added;
    }
    
    // QTNode is not full
    else{
      m_bounds.resize(m_point, pt);
      
      // Looping through all children
      for (int i = 0; i < QT_NUM_KIDS; i++){
	
	// Checks for location to add child
	if (m_child.at(i) == NULL){
	  m_child.at(i) = newChild;
	  m_num_children++;
	  added = true;
	  return added;
	}  
      }
    }
  } 
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
      if(m_child.at(i) != NULL)
        found = m_child.at(i)->find(pt, data);
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
    std::cout << "Internal Node with bounds " << m_bounds << std::endl; 
  else{
    std::cout << "Leaf at Point " << m_point << " with bounds " << m_bounds << " with data " 
	      << m_data << "." << std::endl;
  }

  std::cout << "Children: " << std::endl;

  // Loop is recursive
  for (int i = 0; i < QT_NUM_KIDS; i++){
    if (m_child.at(i) != NULL){

      // Checks if child will have a sibling following
      if(i < QT_NUM_KIDS - 1)
	std::cout << "-> ";
      else
	std::cout << "]]"; 

      m_child.at(i)->dump();
    }
    else
      std::cout << "NULL -> ";

    std::cout << std::endl;
  }
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
  child = parent->m_child.at(m_pos);
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
  child = parent->m_child.at(++m_pos);
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
  return parent->m_child.at(m_pos);
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
// Checks if current node is a leaf
bool QTNode::isLeaf(){

    bool is_leaf = true;
    /*
  // Loops through all children
  for (int i = 0; i < QT_NUM_KIDS; i++){

    // First instance of an existent child is proof of not being a leaf
    if (m_child.at(i) != NULL){
      is_leaf = false;
      return is_leaf;
    }
    }*/
    if (m_num_children > 0)
      is_leaf = false;

  return is_leaf;
}

// Adjusts the bounding boxes of two QTNodes to keep them independent
void QTNode::rescale(QTNode* other){

  // When the points overlap then no rescale occurs
  if( sameX(other) && sameY(other) )
    return;

  // Rescaling is only based on the y coordinate
  else if( sameY(other) ){
    if( largerX(other) ){
      other->m_bounds.resize(other->m_point,Point(m_point.m_x - 1, m_point.m_y));
    }
    else{
    m_bounds.resize(m_point,Point(other->m_point.m_x - 1, other->m_point.m_y));
    }
  }

  // Rescaling is only based on the x coordinate
  else if( sameX(other) ){
   if( largerY(other) ){
      other->m_bounds.resize(other->m_point,Point(m_point.m_x, m_point.m_y - 1));
    }
    else{
    m_bounds.resize(m_point,Point(other->m_point.m_x, other->m_point.m_y - 1));
    }
  }

  // X is definitively larger
  else if( largerX(other) && largerY(other) ){
    other->m_bounds.resize(other->m_point,Point(m_point.m_x - 1, m_point.m_y - 1));
  }

  // Current QTNode has both a smaller x and a smaller y
  else{
    m_bounds.resize(m_point,Point(other->m_point.m_x - 1, other->m_point.m_y - 1));
}
  return;
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
  return m_point.m_y < other->m_point.m_y;
}
bool QTNode::sameX(QTNode* other){
  return m_point.m_x == other->m_point.m_x;
}
bool QTNode::sameY(QTNode* other){
  return m_point.m_y == other->m_point.m_y;
}
