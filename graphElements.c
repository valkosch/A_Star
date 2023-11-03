#include <math.h>
#include "graphElements.h"

double fCost(Node *p){
    return p->gCost + p->hCost;
}
double hCost(Node *A, Node *target){
    return sqrt(pow(A->x - target->x,2) + pow(A->y - target->y,2));
}
