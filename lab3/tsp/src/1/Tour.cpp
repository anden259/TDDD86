// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

// Only need to init startNode in default ctor.
Tour::Tour(): startNode(nullptr) {}

// ctor for testing, creates a tour with four points.
Tour::Tour(Point a, Point b, Point c, Point d)
{
    startNode = new Node(a, new Node(b, new Node(c, new Node(d, nullptr))));
    Node* node = startNode;
    while (node->next != nullptr) {
        node = node->next;
    }
    node->next = startNode;
}

// dtor
Tour::~Tour()
{
    if (startNode == nullptr) {
        return;
    } else if (startNode->next == startNode) {
        delete startNode;
        return;
    }

    Node* current = startNode;
    Node* next = current->next;
    while (next != startNode) {
        next = current->next;
        delete current;
        current = next;
    }
}

// prints the a string representation of all points in the tour.
void Tour::show() const
{
    if (startNode == nullptr) return;

    const Node* currentNode = startNode;
    do {
        cout << currentNode->point.toString() << endl;
        currentNode = currentNode->next;
    } while (currentNode != startNode);
}

// draw all lines between all nodes.
void Tour::draw(QGraphicsScene *scene) const
{
    if (startNode == nullptr) return;

    const Node* currentNode = startNode;
    do {
        currentNode->point.drawTo(currentNode->next->point, scene);
        currentNode = currentNode->next;
    } while (currentNode != startNode);
}

// walks through all nodes to count them.
// a private variable that is increased when adding a Node would be prefered over this.
int Tour::size() const
{
    if (startNode == nullptr) return 0;

    const Node* currentNode = startNode;
    int ret = 0;

    do {
        ++ret;
        currentNode = currentNode->next;
    } while (currentNode != startNode);

    return ret;
}

// walk by all nodes, summing the distance between them.
double Tour::distance() const
{
    if (startNode == nullptr) return 0;

    const Node* node = startNode;
    double ret = 0;

    do {
        ret += node->point.distanceTo(node->next->point);
        node = node->next;
    } while (node != startNode);

    return ret;
}

// insert node after it's closesed node in the tour.
void Tour::insertNearest(Point p)
{
    if (startNode == nullptr) {
        startNode = new Node(p, nullptr);
        startNode->next = startNode;
        return;
    }

    Node* node = startNode;
    double minDist = node->point.distanceTo(p);
    Node* minNode = node;

    node = node->next;
    while (node != startNode) {
        if (minDist > node->point.distanceTo(p)) {
            minDist = node->point.distanceTo(p);
            minNode = node;
        }
        node = node->next;
    }

    Node* newNode = new Node(p, minNode->next);
    minNode->next = newNode;
}

// insert node where it gives the smallest increase in length
void Tour::insertSmallest(Point p)
{
    if (startNode == nullptr) {
        startNode = new Node(p, nullptr);
        startNode->next = startNode;
        return;
    }

    Node* node = startNode;
    double dist = node->point.distanceTo(p) + node->next->point.distanceTo(p) - node->point.distanceTo(node->next->point);
    Node* minNode = node;

    node = node->next;
    while (node != startNode) {
        double newDist = node->point.distanceTo(p) + node->next->point.distanceTo(p) - node->point.distanceTo(node->next->point);
        if (dist > newDist) {
            dist = newDist;
            minNode = node;
        }
        node = node->next;
    }

    Node* newNode = new Node(p, minNode->next);
    minNode->next = newNode;
}
