#include <utility>
#include <cstdlib>
#include <cstdio>
#include "Point.h"
#include "BBox.h"
#include "QTNode.h"
#include "QTree.h"

using namespace std;

int main(int argc, char **argv) {
    
    BBox bBox;
    Point p1, p2;
    QTree myQT;

    // First point added
    myQT.add(Point(4096, 4096), 1);
    // Second pt should interact w/bounds set on first point
    // Should be handled efficiently: minimal node building/destroying
    // Resulting BBox should be {(4096,4096), dim=2}
    myQT.add(Point(4096, 4097), 2);
    myQT.dump();

    return 0;
}
