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

