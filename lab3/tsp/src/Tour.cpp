// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour(): startNode(nullptr)
{

}

Tour::Tour(Point a, Point b, Point c, Point d):startNode(nullptr)
{
    startNode = new Node(a,new Node(b,new Node(c,new Node(d,nullptr))));
    Node* node = startNode;
    while(node->next != nullptr)
    {
        node = node->next;
    }
    node->next = startNode;

}

Tour::~Tour()
{
    // TODO: write this member
}

void Tour::show()
{
    myForeach([] (Node node) { cout << node.point.toString() << endl; });
}

void Tour::draw(QGraphicsScene *scene)
{
    if(startNode == nullptr) return;

    Node* node = startNode;

    do{
        node->point.drawTo(node->next->point,scene);
        node=node->next;
    }while(node != startNode);

}

int Tour::size()
{
    if(startNode == nullptr) return 0;

    Node* node = startNode;
    int ret = 0;

    do{
        ++ret;
        node=node->next;
    }while(node != startNode);

    return ret;
}

double Tour::distance()
{
    if(startNode == nullptr) return 0;

    Node* node = startNode;
    double ret = 0;

    do{
        ret+=node->point.distanceTo(node->next->point);
        node=node->next;
    }while(node != startNode);

    return ret;
}

void Tour::insertNearest(Point p)
{
    if(startNode == nullptr){
        startNode=new Node(p,nullptr);
        startNode->next=startNode;
        return;
    }

    Node* node = startNode;
    double dist = node->point.distanceTo(p);
    Node* minNode=node;

    node = node->next;
    while(node != startNode)
    {
        if(dist > node->point.distanceTo(p))
        {
            dist = node->point.distanceTo(p);
            minNode=node;
        }
        node = node->next;
    }

    Node* newNode=new Node(p,minNode->next);
    minNode->next=newNode;
}

void Tour::insertSmallest(Point p)
{
    if(startNode == nullptr){
        startNode=new Node(p,nullptr);
        startNode->next=startNode;
        return;
    }

    Node* node = startNode;
    double dist = node->point.distanceTo(p) + node->next->point.distanceTo(p) - node->point.distanceTo(node->next->point);
    Node* minNode = node;

    node = node->next;
    while(node != startNode)
    {
        double newDist = node->point.distanceTo(p) + node->next->point.distanceTo(p) - node->point.distanceTo(node->next->point);
        if(dist > newDist)
        {
            dist = newDist;
            minNode = node;
        }
        node = node->next;
    }

    Node* newNode=new Node(p,minNode->next);
    minNode->next=newNode;
}

void Tour::myForeach(void (*f)(Node))
{
    if(startNode == nullptr) return;
    Node* node = startNode;
    do{
        f(*node);
        node=node->next;
    }while(node != startNode);

}

bool Tour::isCrossing(Node *n1, Node *n2)
{
    Point pi0 = n1->point;
    Point pi1 = n1->next->point;
    Point pj0 = n2->point;
    Point pj1 = n2->next->point;

    double xi0,xi1,xj0,xj1,yi0,yi1,yj0,yj1,s,t;
    // pi0
    xi0 = pi0.x;
    yi0 = pi0.y;

    // pi1
    xi1 = pi1.x;
    yi1 = pi1.y;

    // pj0
    xj0 = pj0.x;
    yj0 = pj0.y;

    // pj1
    xj1 = pj1.x;
    yj1 = pj1.y;

    /*
     * - pi0   pj1 -        - pi0 - pj1 -
     *       X        ==>
     * - pj0   pi1 -        - pj0 - pi1 -
     *
     * gives:
     *
     * xi0 + xi1*s = xj0 + xj1*t
     * yi0 + yi1*s = yj0 + yj1*t
     *
     * solve for s and t
     * s and t between 0 and 1 => crossing paths.
     */

    s = (yj1*(xi0-xj0) - yi0*xj1 + yj0*xj1) / (yi1*xj1 - xi1*yj1);
    t = (yi0*xi1 - xi0*yi1 - xi1*yj0 + yi1*xj0) / (xi1*yj1 - yi1*xj1);

    bool res = ((0 < s) && (s < 1) && (0 < t) && (t < 1));
    cout << res << " L1:" << pi0 << "->" << pi1 << " L2:" << pj0 << "->" << pj1 << endl;
    return ((0 < s) && (s < 1) && (0 < t) && (t < 1));
}

/*
       repeat until no improvement is made {
       start_again:
       best_distance = calculateTotalDistance(existing_route)
       for (i = 0; i < number of nodes eligible to be swapped - 1; i++) {
           for (k = i + 1; k < number of nodes eligible to be swapped; k++) {
               new_route = 2optSwap(existing_route, i, k)
               new_distance = calculateTotalDistance(new_route)
               if (new_distance < best_distance) {
                   existing_route = new_route
                   goto start_again
               }
           }
       }
   }
   */

void Tour::routeSwap(Node *n1, Node *n2)
{
    Node* prevNode = n1;
    Node* currentNode = n1->next;
    Node* nextNode = currentNode->next;
    Node* stopNode = n2->next;

    while(currentNode->next != stopNode)
    {
        nextNode = currentNode->next;
        currentNode->next = prevNode;
        prevNode = currentNode;
        currentNode = nextNode;
    }

    n1->next->next = stopNode;
    n1->next = n2;
    cout << "tja" << endl;
}

void Tour::improve()
{
    bool foundCrossing = false;
    unsigned int size = this->size();

    do{
        Node* ni = startNode;
        Node* nj = ni->next;
        // 0 <= ni < nj < this->size()

        foundCrossing = false;
        // Loop through all nodes
        for(unsigned int i = 0; i < size-1; ++i)
        {
            // Loop through all nodes, from the current one to the end
            for(unsigned int j = i; j < size-1; ++j)
            {
                if(isCrossing(ni, nj))
                {
                    //foundCrossing = true;
                    routeSwap(ni, nj);
                }
                nj = nj->next;
            }
            ni = ni->next;
            nj = ni->next;
        }

    }while(foundCrossing);

}
