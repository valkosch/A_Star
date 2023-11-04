
#ifndef ASTAR_GRAPHELEMENTS_H
#define ASTAR_GRAPHELEMENTS_H

typedef struct Edge{
    void *p0, *p1;
    double length, time;
} Edge;

typedef struct Node{
    double x,y;
    int fokszam;
    double gCost, hCost;
    int index;
    struct Node *parent;
    Edge *p;
} Node;
double fCost(Node *p);
double hCost(Node *A, Node *target);

#endif //ASTAR_GRAPHELEMENTS_H
