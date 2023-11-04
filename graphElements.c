#include <math.h>
#include "graphElements.h"

double fCost(Node *p){
    return p->gCost + p->hCost;
}
double hCost(Node *A, Node *target){
    return sqrt(pow(target->x - A->x,2) + pow(target->y - A->y,2));
}
