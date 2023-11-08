

#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H

#include "linkedlist.h"
#include "graphElements.h"
void Pathfinder(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb);
void SetStart(Node *startNode, Node *targetNode);
#endif //ASTAR_PATHFINDER_H
