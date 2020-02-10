/*****************************************
 ** File:    QTree.h
 ** Project: CMSC 341 Project 3 - QuadTrees, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    04/10/18
 ** Section: 03
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of the QTree class
 **   This class handles creating and managing the quad tree 
 **    and all it's different operations
 ***********************************************/

#ifndef QTREE_H
#define QTREE_H

#include <vector>
#include "Point.h"
#include "BBox.h"
#include "QTNode.h"

class QTree {
public:
    QTree();
    ~QTree();
    bool add(const Point &pt, int data);  // actually, add/replace
    bool remove(const Point &pt);
    bool find(const Point &pt, int &data);
    int findPoints(const BBox &region, std::vector<Point> &found);
    void dump();

    QTNode *m_root;
    int m_size;

private:

    // Helper Functions for Add
    bool emptyQuad(QTNode* child, int quad);
    bool helpOccupied(const Point &pt, int data, QTNode* child, int quad);
    void growRoot(const Point &pt);
};

#endif
