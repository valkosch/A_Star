#include <math.h>
#include "graphElements.h"

double fCost(Node *p){ /*fCost az A*-ban az adott pont prioritását mondja meg*/
    return p->gCost + p->hCost;
}
double hCost(Node *A, Node *target){ /*a hCost 2 egy síkban lévő pont közötti távolságot adja meg*/
    return sqrt(pow(target->x - A->x,2) + pow(target->y - A->y,2));
}
