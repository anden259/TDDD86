// anden259 andno037

#include <algorithm>
#include <iostream>
#include "costs.h"
#include "pqueue.h"
#include "trailblazer.h"
#include <queue>

using namespace std;

// Recursive help function for DFS
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
                // If the child finds a way, add it to the path list.
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

// DFS
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end)
{
    vector<Node *> path;
    graph.resetData();
    depthFirstSearch_help(graph,  start, end, path);
    return path;
}

// BFS
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
            // We found the goal, rebuild the path all the way back to the start point
            while (current != nullptr) {
                path.push_back(current);
                current = current->previous;
            }
            // reverse the path vector, so that it starts from the begning.
            reverse(path.begin(), path.end());
            return path;
        }

        // Else, pop the queue and start the work on the next unvisited neigbour.
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
    PriorityQueue<Vertex*> openset; // The set of nodes to be evaluated, starting with our location
    openset.enqueue(start, 0.0); // Add the startpoint, with 0 cost.
    start->previous = nullptr; // Set the previous to nullptr, so that we can make sure we never back longer then this.

    while (!openset.isEmpty()) { // While we havn't check all possible paths
        Vertex* current = openset.dequeue();
        current->visited = true;
        current->setColor(GREEN);

        if (current == end) {
            // Hit the goal, rebuild the path and return it.
            while (current != nullptr) {
                path.push_back(current);
                current = current->previous;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Check all edges from the current node.
        for (Edge* edge : graph.getEdgeSet(current)) {
            Vertex* neighbour;
            neighbour = edge->finish; // get the node on the other side of the edge.

            // If the neigbour's already dealt with, continue to the next.
            if (neighbour->visited)
                continue;

            bool open = false;
            // Check if the node is in the openset,
            // by checking if we have any edges to it have been traversed.
            for (Edge* e : neighbour->arcs) {
                if (e->visited) {
                    open = true;
                    break;
                }
            }


            edge->visited = true; // mark our edge as visited.
            graph.getEdge(neighbour, current)->visited = true; // from the other side as well.


            double temp_cost = current->cost + edge->cost;
            // if the neighbour isn't in the openset, add it to the openset, with the apropiate cost.
            if (!open) {
                neighbour->previous = current;
                neighbour->cost = temp_cost;
                openset.enqueue(neighbour, temp_cost);
                neighbour->setColor(YELLOW);
            // else update the cost, if we have found a cheaper path.
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
    PriorityQueue<Vertex*> openset; // The set of nodes to be evaluated, starting with our location
    openset.enqueue(start, 0.0); // Add the startpoint, with 0 cost.
    start->previous = nullptr; // Set the previous to nullptr, so that we can make sure we never back longer then this.

    while (!openset.isEmpty()) { // While we havn't check all possible paths
        Vertex* current = openset.dequeue();
        current->visited = true;
        current->setColor(GREEN);

        if (current == end) {
            // Hit the goal, rebuild the path and return it.
            while (current != nullptr) {
                path.push_back(current);
                current = current->previous;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Check all edges from the current node.
        for (Edge* edge : graph.getEdgeSet(current)) {
            Vertex* neighbour;
            neighbour = edge->finish; // get the node on the other side of the edge.

            // If the neigbour's already dealt with, continue to the next.
            if (neighbour->visited)
                continue;

            bool open = false;
            // Check if the node is in the openset,
            // by checking if we have any edges to it have been traversed.
            for (Edge* e : neighbour->arcs) {
                if (e->visited) {
                    open = true;
                    break;
                }
            }

            edge->visited = true; // mark our edge as visited.
            graph.getEdge(neighbour, current)->visited = true; // from the other side as well.

            double temp_cost = current->cost + edge->cost;
            double priority_cost = temp_cost + neighbour->heuristic(end);
            // if the neighbour isn't in the openset, add it to the openset, with the estimated cost.
            if (!open) {
                neighbour->previous = current;
                neighbour->cost = temp_cost;
                openset.enqueue(neighbour, priority_cost);
                neighbour->setColor(YELLOW);
            // else if we have found a cheaper estimate, update the cost with that estimate.
            } else if (temp_cost < neighbour->cost) {
                neighbour->previous = current;
                neighbour->cost = temp_cost;
                openset.changePriority(neighbour, priority_cost);
            }
        }
    }
    return path;
}
