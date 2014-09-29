// This is the .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header

#ifndef TOUR_H
#define TOUR_H

#include <vector>
#include "Node.h"
#include "Point.h"

//template <typename T>
class Tour
{
public:

    Tour();
    Tour(Point a, Point b, Point c, Point d);
    ~Tour();
    void show() const;
    void draw(QGraphicsScene* scene) const;
    int size() const;
    double distance() const;
    void insertNearest(Point p);
    void insertSmallest(Point p);
    void insertPoint(Point p);
    void doFarthest();
    void improve();

private:
    Node* startNode;
    std::vector<Point> point_vec;

    bool isCrossing(const Node *n1, const Node *n2) const;
    void routeSwap(Node* n1, Node* n2);
    std::vector<Point>::iterator findFarthest();
    void startFarthest();
};

#endif // TOUR_H
