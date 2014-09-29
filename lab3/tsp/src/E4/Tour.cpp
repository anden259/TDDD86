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

// Check if the lines between the points is crossing
bool Tour::isCrossing(const Node *n1, const Node *n2) const
{
    // This is only equation solving
    const Point pi0 = n1->point;
    const Point pi1 = n1->next->point;
    const Point pj0 = n2->point;
    const Point pj1 = n2->next->point;

    double a, b, c, d, e, f, g, h, s, t, div;

    a = pi0.x;
    b = pi0.y;

    c = pi1.x;
    d = pi1.y;

    e = pj0.x;
    f = pj0.y;

    g = pj1.x;
    h = pj1.y;

    /* Identify the following:
     *
     * - pi0   pj1 -
     *       X
     * - pj0   pi1 -
     *
     * gives:
     *
     * a + (c-a)*s = e + (g-e)*t
     * b + (d-b)*s = f + (h-e)*t
     *
     * solve for s and t,
     * s and t between 0 and 1 => crossing paths.
     */

    div = ((a - c) * (f - h) + b * (g - e) + d * (e - g));
    s = (a * (f - h) + b * (g - e) + e * h - f * g) / div;
    t = (a * (f - d) + b * (c - e) - c * f + d * e) / div;

    return ((0 < s) && (s < 1) && (0 < t) && (t < 1));
}

// disolve crossings by swaping nodes and reverses the node direction between them.
void Tour::routeSwap(Node *n1, Node *n2)
{
    /*
     *      ->  n1   n2  <-   ...    <-
     *             X                   |
     * <- n2->next   n1->next -> ... ->
     *
     * gives:
     *
     *     ->  n1  -> n2   ->   ...   ->
     *                                   |
     * <- n2->next <- n1->next <- ... <-
     *
     */

    Node* prevNode = n1;
    Node* currentNode = n1->next;
    Node* nextNode = currentNode->next;
    Node* stopNode = n2->next;

    while (currentNode != stopNode) {
        nextNode = currentNode->next;
        currentNode->next = prevNode;
        prevNode = currentNode;
        currentNode = nextNode;
    }

    n1->next->next = stopNode;
    n1->next = n2;
    return;
}

// Remove all crossings.
void Tour::improve()
{
    bool foundCrossing = false;
    unsigned int size = this->size();

    // swapRoute if a crossing is found.
    // Loop until no more crossings are found.
    do {
        Node* ni = startNode;
        Node* nj = ni->next;

        foundCrossing = false;
        // Loop through all nodes
        // 0 <= ni < nj < this->size()
        for (unsigned int i = 0; i < size - 1; ++i) {
            // Loop through all nodes, from the current one to the end
            for (unsigned int j = i; j < size; ++j) {
                if (isCrossing(ni, nj)) {
                    foundCrossing = true;
                    routeSwap(ni, nj);
                }
                nj = nj->next;
            }
            ni = ni->next;
            nj = ni->next;
        }
    } while (foundCrossing);
}

// insert all points into a private vector
void Tour::insertPoint(Point p)
{
    point_vec.push_back(p);
}

// Find the point from the input that farthest away from all other points.
std::vector<Point>::iterator Tour::findFarthest()
{
    if (point_vec.size() == 1)
        return point_vec.begin();

    std::vector<Point>::iterator maxP;
    double maxDist;

    const Node* node;
    double tmp_minDist;

    for (std::vector<Point>::iterator it = point_vec.begin(); it != point_vec.end(); ++it) {
        node = startNode;
        tmp_minDist = node->point.distanceTo(*it);

        do {
            if (tmp_minDist > node->point.distanceTo(*it)) {
                tmp_minDist = node->point.distanceTo(*it);
            }
            node = node->next;
        } while (node != startNode);

        if (maxDist < tmp_minDist) {
            maxDist = tmp_minDist;
            maxP = it;
        }
    }

    return maxP;
}

// Insert the two points that are farthest away from each other into the Tour
void Tour::startFarthest()
{
    if(point_vec.size() < 2)
        return;

    auto p1 = point_vec.begin();
    auto p2 = p1;
    double maxDist = 0;

    for (auto it1 = point_vec.begin(); it1 != point_vec.end(); ++it1) {
        for (auto it2 = it1; it2 != point_vec.end(); ++it2) {
            if (maxDist < (*it1).distanceTo(*it2)) {
                maxDist = (*it1).distanceTo(*it2);
                p1 = it1;
                p2 = it2;
            }
        }
    }

    insertSmallest(*p1);
    insertSmallest(*p2);
    point_vec.erase(p1);
    point_vec.erase(p2);
}

// Create the tour with the "Farthest heuristic"
void Tour::doFarthest()
{
    startFarthest();

    std::vector<Point>::iterator it;
    while (!point_vec.empty()) {
        it = findFarthest();
        insertSmallest(*it);
        point_vec.erase(it);
    }
}
