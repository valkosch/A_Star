

#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H

#include "linkedlist.h"
#include "graphElements.h"
void retrace(List *path, Node *start, Node *end);
void Pathfinder(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb);

#endif //ASTAR_PATHFINDER_H
