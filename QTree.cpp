/*****************************************
 ** File:    QTree.cpp
 ** Project: CMSC 341 Project 3 - QuadTrees, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    04/10/18
 ** Section: 03
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of the QTree class
 **   This class manages all functions of the Quad Tree such as addition of new nodes,
 **     node removal, finding of nodes, etc.
 **
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
  int leaf = 1;
  QTNode* new_child = new QTNode(pt, leaf, data);

  if (m_root != NULL)
    m_root->m_leaves++;

  // Edge case for adding first node
  if (m_size == 0){
    m_root = new_child;
    added = true;
    m_size++;
    return added;
  }

  // Checks if pointed added is a duplicate
  // Only checks leaves
  else if( (m_root->m_point == pt) && (m_root->m_num_children < 1) ){
    m_root->m_data = data;
    m_root->m_leaves--;

    // Must clean up memory for unused child
    delete new_child;
    new_child = NULL;
    return added;
  }  

  // Edge case for adding second node
  else if (m_size == 1) {
 
    // Setting a new root
    QTNode* old_root = m_root;
    m_root = new QTNode();
    m_root->m_bounds.resize(old_root->m_point,pt);
    m_root->m_point = old_root->m_point;
    m_root->m_leaves = old_root->m_leaves;

    // Updating member variables
    int n_child_quad = m_root->getQuad(pt);
    int o_root_quad = m_root->getQuad(old_root->m_point);

    // Alligning nodes in tree
    m_root->m_child[n_child_quad] = old_root;
    m_root->m_num_children++;
    m_root->m_child[o_root_quad] = new_child;
    m_root->m_num_children++;
    added = true;
    m_size++;
    return added;
  }

  // All other cases
  
  // Root is about to become full
  // Root's nodes must be split
  // growRoot() can be exploited by using a point that would cause minimum growth
  //  because growRoot also splits the root
  if (m_root->m_num_children == 4){
    const unsigned int MIN_GROWTH = m_root->m_bounds.m_dim;
    const Point GROWER(m_root->m_point.m_x+MIN_GROWTH,m_root->m_point.m_y+MIN_GROWTH);
    growRoot(GROWER);
    // Fallthrough
    }
  
  // Checking if root's current bounds are large enough
  if( m_root->m_bounds.inBounds(pt) ){
    int quad = m_root->getQuad(pt);
    
    // Quadrant adding to is empty
    if (m_root->m_child[quad] == NULL){ 
      added = emptyQuad(new_child, quad);
      return added;
    }
    
    // Quadrant is occupied
    added = helpOccupied(pt, data, new_child, quad);
    return added;     
  }
  
  // Bounds of root are not large enough to fit new node
  else{
    growRoot(pt); 
    int quad = m_root->getQuad(pt);
   
    // Quad to add child is empty
    if (m_root->m_child[quad] == NULL){
      added = emptyQuad(new_child, quad);
      return added;
    }
    
    // Quad is occupied 
    // So add to current child or
    //  current child needs to be grown
    added = helpOccupied(pt, data, new_child, quad);
    return added;  
  }
}

// Removes QTNodes from the quad tree
// Only leaves can be removed
// Returns whether or not the defined node was removed
bool QTree::remove(const Point &pt){

  bool removed = false;
  bool empty = false;

  // Tree has not been created
  if (m_root == NULL)
    removed = false;

  // Point removing is the root
  else if( (m_root->m_point == pt) && (m_root->m_num_children < 1) ){
    removed = true;
    delete m_root;
    m_root = NULL;
    m_size--;
    return removed;
  }

  // Point is likely within the bounds of the root
  else if (m_root->m_bounds.inBounds(pt)){
    int quad = m_root->getQuad(pt);

    // Base Case, when area where point should be is empty
    if (m_root->m_child[quad] == NULL)
      removed = false;

    // Child Node is the one needing removal
    else if( (m_root->m_child[quad]->m_point == pt) && 
	     (m_root->m_child[quad]->m_num_children < 1) ){

      delete m_root->m_child[quad];
      m_root->m_child[quad] = NULL;
      m_root->m_num_children--;
      m_root->m_leaves--;
      removed = true;
      
      // Root cannot be left with only one child
      // Runs until a root with two or more children is found
      while( (m_root != NULL) && (m_root->m_num_children == 1) ){
	const int MAX_CHILDREN = 4;
	int count = 0;
	  
	// Determines which node to promote
	while( (count < MAX_CHILDREN) && (m_root->m_child[count] == NULL) )
	  count++;

	// Error checking for empty child
	if (m_root->m_child[count] != NULL){
	  QTNode* temp = m_root->m_child[count];
	  m_root->m_child[count] = NULL;
	  delete m_root;
	  m_root = temp;
	}
      }
    }
  
  // Recursive case, when need to look thorugh next child for point
  else
    removed = m_root->m_child[quad]->remove(pt,empty); 
}

  // If not within the bounds of the root the point does not exist
  if(removed){
    m_size--;
    m_root->m_leaves--;
  }
  return removed;
}

// Finds QTNodes in the tree
// Returns whether or not the defind node was found
// Internal nodes (that don't have valid points) have a data of 0
//  They only have points to use
bool QTree::find(const Point &pt, int &data){

  bool found = false;

  // Tree has not been created
  if (m_root == NULL)
    found = false;

  // Point looking for is the root
  else if( (m_root->m_point == pt) && (m_root->m_num_children < 1) ){
    found = true;
    data = m_root->m_data;
  }

  // Point is likely within the bounds of the root
  else if (m_root->m_bounds.inBounds(pt)){
    int quad = m_root->getQuad(pt);

    // Base Case, when area where point should be is empty
    if (m_root->m_child[quad] == NULL)
      found = false;

    // Recursive case, when need to look thorugh next child for point
    else
      found = m_root->m_child[quad]->find(pt, data); 
  }

  // If not within the bounds of the root the point does not exist
  return found;
}

// Reports all points found within a specified region
int QTree::findPoints(const BBox &region, std::vector<Point> &found){

  int num_points = 0;

  // Creating some variables to help search work with rules of
  //  the Quad Tree sizes
  unsigned int region_max_x = region.m_bL.m_x + region.m_dim; 
  unsigned int region_max_y = region.m_bL.m_y + region.m_dim; 
  Point region_max(region_max_x,region_max_y);
  BBox temp(region.m_bL, region.m_dim);
  temp.resize(region.m_bL, region_max);
  
  // Tree has not been created
  if (m_root == NULL)
    return num_points;

  // Region looking for is the root
  else if (m_root->m_bounds.m_bL == region.m_bL){

    // When root is a leaf   
    if (m_root->m_num_children < 1){
      found.push_back(m_root->m_point);
      num_points++;
    }

    // Root is not a leaf
    // Adds up all the leaves within the tree
    else{
      const int MAX_CHILDREN = 4;

      // Appends all leaves to found vector
      for (int i = 0; i < MAX_CHILDREN; i++)
	if (m_root->m_child[i] != NULL)
	  num_points += m_root->m_child[i]->findPoints(m_root->m_child[i]->m_bounds,found);
    }
  }

  // Region searching for is within the tree
  else if( (m_root->m_bounds.inBounds(region.m_bL)) || 
	   (region.inBounds(m_root->m_bounds.m_bL)) ){

    int quad = m_root->getQuad(region.m_bL);

    // Base Case, when area looking for is empty
    if (m_root->m_child[quad] == NULL)
      return num_points;

    // Recursive case, when need to look thorugh next child for point
    else
      num_points += m_root->m_child[quad]->findPoints(temp,found, region_max, region.m_bL);  
  }
  
  // If not within the bounds of the root the point does not exist
  return num_points;
}

// Debugging function
void QTree::dump(){
  std::cout << "============================================================" << std::endl;
  std::cout << "START Dump of Quad Tree" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;

  // Error checking for empty tree
  if (m_root != NULL)
    m_root->dump();
  else 
    std::cout << "Empty Quad Tree" << std::endl;

  std::cout << "------------------------------------------------------------" << std::endl;
  std::cout << "END Dump of Quad Tree" << std::endl;
  std::cout << "============================================================" << std::endl;
}

// Helps to add a node to empty child of root
bool QTree::emptyQuad(QTNode* child, int quad){
  m_root->m_child[quad] = child;
  m_root->m_num_children++;
  bool added = true;
  m_size++;
  return added;
}

// Helps to add a node to a filled quadrant
bool QTree::helpOccupied(const Point &pt, int data, QTNode* child, int quad){
  bool added = false;

  // Checks if node is within bounds of the current child in this location
  if( m_root->m_child[quad]->m_bounds.inBounds(pt) ){
    added = m_root->m_child[quad]->add(pt, data);

    // Updating members
    if(added){
      m_size++;
    }

    // Cleaning up memeory
    delete child;
    child = NULL;
    return added;
  }

  // Bounds of the current quadrant's child need to be grown
  QTNode* new_parent = new QTNode();
  QTNode* old_parent = m_root->m_child[quad];
  
  // Resizing new parent and determining new node locations
  new_parent->m_bounds.resize(pt, old_parent->m_point);
  new_parent->m_point = new_parent->m_bounds.m_bL;
  new_parent->m_leaves = old_parent->m_leaves;
  int child_quad = new_parent->getQuad(pt);
  int o_parent_quad = new_parent->getQuad(old_parent->m_point);
  
  // Placing nodes in their proper locations
  new_parent->m_child[child_quad] = old_parent;
  new_parent->m_num_children++;  
  new_parent->m_child[o_parent_quad] = child;
  new_parent->m_num_children++;
  m_root->m_child[quad] = new_parent;

  //updating members
  m_size++;
  added = true;
  return added;
}

// Helps to grow the root in order to add new child
void QTree::growRoot(const Point &pt){

  // Checks if root needs to be repositioned (in order to fit squareness)
  if( (m_root->m_bounds.m_dim == m_root->m_bounds.m_bL.m_x) || 
      (m_root->m_bounds.m_dim == m_root->m_bounds.m_bL.m_y) ){

    Point zero(0,0);

    // Setting up a new root with a larger dimension
    QTNode* old_root = m_root;
    QTNode* new_root = new QTNode(zero,old_root->m_bounds);
    new_root->m_bounds.m_bL = zero;
    new_root->m_bounds.m_dim = old_root->m_bounds.m_dim;
    new_root->m_bounds.grow();
    new_root->m_leaves = old_root->m_leaves;

    // Re-adjusting root and new root
    new_root->m_child[3] = old_root;
    new_root->m_num_children++;
    m_root = new_root;
  }

  // Root has not broken squareness
  else{

    // Setting up a new root with a larger dimension
    QTNode* old_root = m_root;
    QTNode* new_root = new QTNode(old_root->m_bounds);
    new_root->m_bounds.grow();
    new_root->m_leaves = old_root->m_leaves;
    
    // Re-adjusting root and new root
    new_root->m_child[0] = old_root;
    new_root->m_num_children++;
    m_root = new_root;
  }

  // Root has grown enough
  if( m_root->m_bounds.inBounds(pt) )
    return;

  // Recursively grow the root until large enough to fit point
  else
    growRoot(pt);
}
