#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"
#include "pathfinder.h"
#include "beolvasas.h"




int main() {
    Node a = {1,1,2, 0, sqrt(58), 0, NULL, malloc(2*sizeof(Edge))};
    Node c = {4,8,3, INT_MAX, INT_MAX, 1, NULL,malloc(3*sizeof(Edge))};
    Node d = {4,1,3, INT_MAX, INT_MAX,2 ,NULL, malloc(3*sizeof(Edge))};
    Node e = {8,4,2, INT_MAX, INT_MAX, 3,NULL, malloc(2*sizeof(Edge))};

    a.p[0].length = 3;
    a.p[1].length = sqrt(18);
    a.p[0].p1 = &d;
    a.p[1].p1 = &c;

    c.p[0].length = sqrt(58);
    c.p[1].length = 3;
    c.p[2].length = 7;
    c.p[0].p1 = &a;
    c.p[1].p1 = &d;
    c.p[2].p1 = &e;

    d.p[0].length = 5;
    d.p[1].length = 7;
    d.p[2].length = 3;
    d.p[0].p1 = &e;
    d.p[1].p1 = &c;
    d.p[2].p1 = &a;

    e.p[0].length = sqrt(32);
    e.p[1].length = 5;
    e.p[0].p1 = &c;
    e.p[1].p1 = &d;

    Node tomb[] ={a, c, d, e};
    int nodeNum;
    Node *test = NodeInput(&nodeNum);
    EdgeInput(test, &nodeNum);
    List openSet = {NULL, NULL};
    List closedSet = {NULL, NULL};
    List path = {NULL, NULL};
    Pathfinder(&a, &e, &openSet, &closedSet, tomb);
    retrace(&path, &a, &e);


    /*SetStart(&test[0], &test[50]);
    Pathfinder(&test[0], &test[50], &openSet, &closedSet, test);
    retrace(&path, &test[0], &test[50]);*/

    FreeNodes(nodeNum, test);
    DeleteList(&path);
    DeleteList(&openSet);
    DeleteList(&closedSet);
    free(a.p);
    free(c.p);
    free(d.p);
    free(e.p);
    return 0;
}
