/*****************************************
 ** File:    QTNode.h
 ** Project: CMSC 341 Project 3 - QuadTrees, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    04/10/18
 ** Section: 03
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of the QTNode class
 **   This class is a node class that acts as a subtree for the
 **    quad tree (each node consisting of 4 or less children)
 **   Manages all operations done on nodes within its subtree
 ***********************************************/

#ifndef QTNODE_H
#define QTNODE_H

#include <vector>
#include "Point.h"
#include "BBox.h"

#define QT_NUM_KIDS 4

class QTNode {
public:
    class iterator {
    public:
	iterator();
	iterator(QTNode* node, int pos = 0);
	bool operator==(const QTNode::iterator &other);
	bool operator!=(const QTNode::iterator &other);
	iterator &operator++();          // Prefix: e.g. "++it"
	iterator operator++(int dummy);  // Postfix: "it++"
	QTNode *&operator*();

    private:
	// Add data members here to record state of current iterator position
	QTNode *child;
	QTNode *parent;
	int m_pos;
    };

    QTNode();
    ~QTNode();
    bool add(const Point &pt, int data);  // actually, add/replace
    bool remove(const Point &pt, bool &empty);
    bool find(const Point &pt, int &data);
    int findPoints(const BBox &region, std::vector<Point> &found);
    void dump();

    iterator begin();
    iterator end();

    BBox m_bounds;  // The bounding box for this node
    Point m_point;  // If leaf node (i.e., no kids), m_point, m_data hold the
    int m_data;     // actual point and data value the user inserted.

    // YOu must add data member(s) to store pointers to child QTNode's

    // Added members:
    QTNode** m_child;
    int m_num_children;
    int m_leaves;
    bool m_dummy;

    // YOU CAN ADD ANY OTHER CONSTRUCTORS, MEMBER FUNCTIONS, MEMBER DATA, ETC.
    // AS NEEDED.

    // Overloaded Constructors
    QTNode(bool dummy);
    QTNode(const Point &pt, int leaves = 0, int data = 0);
    QTNode(BBox bounds, int leaves = 0, int data = 0);
    QTNode(const Point &pt, BBox bounds, int leaves = 0, int data = 0);

    // Helper functions
    int findPoints(const BBox &region, std::vector<Point> &found, const Point &max, const Point &min);
    int getQuad(const Point &pt);
    Point getMid();
    Point getMax();
};
#endif
