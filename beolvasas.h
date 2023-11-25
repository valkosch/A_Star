
#ifndef ASTAR_BEOLVASAS_H
#define ASTAR_BEOLVASAS_H
#include "graphElements.h"
typedef struct mapInfo{
    double north, south, east, west;
    int imgWidth, imgHeight;
}mapInfo;
mapInfo GetMapI(void);
void XYconvert(double *x, double *y);
void mapItNodes(NodeTomb *mainArray);
void NodeInput(NodeTomb *mainArray);
void NodeInit(Node *a);
void FreeNodes(NodeTomb* mainArray);
void EdgeInput(NodeTomb *mainArray);
void LineString(Vector2Cell **list0, Vector2Cell **list1, char* str);
void LineStringSpec(Vector2Cell **list0, char* str);
#endif //ASTAR_BEOLVASAS_H
