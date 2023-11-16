

#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H

#include "linkedlist.h"
#include "graphElements.h"
void PathfinderS(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb);
void PathfinderF(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb);
void SetStartS(Node *startNode, Node *targetNode);
void SetStartF(Node *startNode, Node *targetNode);
#endif //ASTAR_PATHFINDER_H
