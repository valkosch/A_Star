

#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H

#include "linkedlist.h"
#include "output.h"

void Pathfinder(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb, ButtonType type);
void SetStart(Node *startNode, Node *targetNode, ButtonType type);
void FindNearest_Node(int clickX, int clickY, NodeTomb *mainArray, Node **A);
#endif //ASTAR_PATHFINDER_H
