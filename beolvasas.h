
#ifndef ASTAR_BEOLVASAS_H
#define ASTAR_BEOLVASAS_H
#include "graphElements.h"
typedef struct mapInfo{
    double north, south, east, west;
    int imgWidth, imgHeight;
}mapInfo;
mapInfo GetMapI(void);
void mapItNodes(Node *tomb, int const *l);
Node *NodeInput(int *nodeNum);
void NodeInit(Node *a);
void FreeNodes(int l, Node *t);
void EdgeInput(Node *tomb, int const *m);
#endif //ASTAR_BEOLVASAS_H
