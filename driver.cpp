#include "QTree.h"
#include "Point.h"
#include "QTNode.h"
#include <iostream>

using namespace std;

/*
  Driver for QuadTree to test functions
 */
int main(){

  QTree myQT;

  myQT.add(Point(4096, 4096), 1);
  // Second pt should interact w/bounds set on first point
  // Should be handled efficiently: minimal node building/destroying
  // Resulting BBox should be {(4096,4096), dim=2}
  myQT.add(Point(4096, 4097), 2);
  myQT.dump();
  // Third point should just add into available quadrant; root should be same
  myQT.add(Point(4097, 4097), 3);
  myQT.dump();

  // Trigger tree expansion by exceeding bounds of root;
  // new BBox should be {(4096, 4096), dim=4
  myQT.add(Point(4098, 4098), 10);
  myQT.dump();

  for(QTNode::iterator it = myQT.m_root->begin(); it != myQT.m_root->end(); it++){
    if (*it != NULL)
      cout << (*it)->m_point << endl;
    else
      cout << "NULL" << endl;
  }
}
