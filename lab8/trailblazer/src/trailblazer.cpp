// anden259 andno037

#include <algorithm>
#include <iostream>
#include "costs.h"
#include "pqueue.h"
#include "trailblazer.h"
#include <queue>

using namespace std;
bool depthFirstSearch_help(BasicGraph& graph, Vertex* start, Vertex* end, vector<Node *>& path)
{
    start->visited = true;
    start->setColor(GREEN);
    if (start == end) {
        path.push_back(start);
        return true;
    } else {
        for (Node* node : graph.getNeighbors(start)) {
            if (!node->visited) {

                if (depthFirstSearch_help(graph,  node, end, path)) {
                    path.push_back(start);
                    return true;
                }
            }
        }
        start->setColor(GRAY);
        return false;
    }
}
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end)
{
    vector<Node *> path;
    graph.resetData();
    depthFirstSearch_help(graph,  start, end, path);
    return path;
}

vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end)
{
    graph.resetData();
    queue<Node *> to_go_to;
    to_go_to.push(start);
    vector<Vertex*> path;
    while (!to_go_to.empty()) {
        Vertex* current = to_go_to.front();
        current->setColor(GREEN);
        current->visited = true;
        if (current == end) {
            while (current != nullptr) {
                path.push_back(current);
                current = current->previous;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        to_go_to.pop();
        for (Node* node : graph.getNeighbors(current)) {
            if (!node->visited) {
                node->previous = current;
                to_go_to.push(node);
                node->setColor(YELLOW);
            }

        }
    }
    return path;
}

vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end)
{
    graph.resetData();
    vector<Vertex*> path;
    PriorityQueue<Vertex*> openset;// The set of nodes to be evaluated, starting with our location
    openset.enqueue(start, 0.0);
    start->previous = nullptr;

    while (!openset.isEmpty()) {
        Vertex* current = openset.dequeue();
        current->visited = true;
        current->setColor(GREEN);

        if (current == end) {
            while (current != nullptr) {
                path.push_back(current);
                current = current->previous;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (Edge* edge : graph.getEdgeSet(current)) {
            Vertex* neighbour;
            neighbour = edge->finish;

            if (neighbour->visited)
                continue;

            bool open = false;
            for (Edge* e : neighbour->arcs) {
                if (e->visited) {
                    open = true;
                    break;
                }
            }

            edge->visited = true;
            graph.getEdge(neighbour, current)->visited = true;

            double temp_cost = current->cost + edge->cost;
            //double priority_cost = temp_cost + neighbour->heuristic(end);
            if (!open) {
                neighbour->previous = current;
                neighbour->cost = temp_cost;
                openset.enqueue(neighbour, temp_cost);
                neighbour->setColor(YELLOW);
            } else if (temp_cost < neighbour->cost) {
                neighbour->previous = current;
                neighbour->cost = temp_cost;
                openset.changePriority(neighbour, temp_cost);
            }
        }
    }
    return path;
}

vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end)
{
    graph.resetData();
    vector<Vertex*> path;
    PriorityQueue<Vertex*> openset;// The set of nodes to be evaluated, starting with our location
    openset.enqueue(start, 0.0);
    start->previous = nullptr;

    while (!openset.isEmpty()) {
        Vertex* current = openset.dequeue();
        current->visited = true;
        current->setColor(GREEN);

        if (current == end) {
            while (current != nullptr) {
                path.push_back(current);
                current = current->previous;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (Edge* edge : graph.getEdgeSet(current)) {
            Vertex* neighbour;
            neighbour = edge->finish;

            if (neighbour->visited)
                continue;

            bool open = false;
            for (Edge* e : neighbour->arcs) {
                if (e->visited) {
                    open = true;
                    break;
                }
            }

            edge->visited = true;
            graph.getEdge(neighbour, current)->visited = true;

            double temp_cost = current->cost + edge->cost;
            double priority_cost = temp_cost + neighbour->heuristic(end);
            if (!open) {
                neighbour->previous = current;
                neighbour->cost = temp_cost;
                openset.enqueue(neighbour, priority_cost);
                neighbour->setColor(YELLOW);
            } else if (temp_cost < neighbour->cost) {
                neighbour->previous = current;
                neighbour->cost = temp_cost;
                openset.changePriority(neighbour, priority_cost);
            }
        }
    }
    return path;
}
