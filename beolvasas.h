
#ifndef ASTAR_BEOLVASAS_H
#define ASTAR_BEOLVASAS_H
#include "graphElements.h"
Node *NodeInput(int *nodeNum);
void NodeInit(Node *a);
void FreeNodes(int l, Node *t);
void EdgeInput(Node *tomb, int const *m);
#endif //ASTAR_BEOLVASAS_H
