
#ifndef ASTAR_GRAPHELEMENTS_H
#define ASTAR_GRAPHELEMENTS_H
typedef struct Vector2{
    int x, y;
}Vector2;
typedef struct Vector2Double{
    double x, y;
}Vector2Double;

typedef struct Vector2Cell {
    struct Vector2Cell *next;
    Vector2Double data;
}Vector2Cell;

typedef struct Edge{
    void  *p1;
    double length, time;
    Vector2Cell *linestring;
} Edge;

typedef struct Node{
    double x,y;
    int fokszam;
    double gCost, hCost;
    int index;
    struct Node *parent;
    Edge *p;
} Node;
typedef struct NodeTomb{
    Node *nodes;
    int NodeNum;
}NodeTomb;

double fCost(Node *p);
double hCost(Node *A, Node *target);

#endif //ASTAR_GRAPHELEMENTS_H
