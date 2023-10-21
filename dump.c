#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct Edge{
    void *p0, *p1;
    double hossz, gyorsasag;
} Edge;

typedef struct Node{
    double x,y;
    int fokszam;
    double gCost, hCost;
    struct Node *parent;
    Edge *p;
} Node;

typedef struct ListaElem {
    struct ListaElem *elozo, *kovi;
    Node adat;
} ListaElem;

typedef struct Lista {
    ListaElem *elso, *utolso;
} Lista;

ListaElem *hozzafuzSpec(Lista *lista, Node *uj_adat, ListaElem *uj) {
    uj = malloc(sizeof(ListaElem));
    uj->adat = *uj_adat;
    uj->kovi = NULL;

    if (lista->utolso != NULL) // ha a lista nem üres
        lista->utolso->kovi = uj;
    else
        lista->elso = uj;

    uj->elozo = lista->utolso;
    lista->utolso = uj;
    return uj;
}
void hozzafuz(Lista *lista, Node *uj_adat) {
    ListaElem *uj = malloc(sizeof(ListaElem));
    uj->adat = *uj_adat;
    uj->kovi = NULL;

    if (lista->utolso != NULL) // ha a lista nem üres
        lista->utolso->kovi = uj;
    else
        lista->elso = uj;

    uj->elozo = lista->utolso;
    lista->utolso = uj;
}
int Count(Lista *lista){
    int x = 0;
    ListaElem y;
    y.kovi = lista ->elso;
    while( y.kovi!= NULL){
        x++;
        y.kovi = y.kovi->kovi;
    }
    return x;
}
void letorol(Lista *lista, ListaElem *torlendo) {
    if (torlendo != lista->elso)
        torlendo->elozo->kovi = torlendo->kovi;
    else
        lista->elso = torlendo->kovi;

    if (torlendo != lista->utolso)
        torlendo->kovi->elozo = torlendo->elozo;
    else
        lista->utolso = torlendo->elozo;

    free(torlendo);
}
void kiurit(Lista *lista) {
    while (lista->elso != NULL)
        letorol(lista, lista->elso);
}
double fCost(Node *p){
    return p->gCost + p->hCost;
}
double hCost(Node *A, Node *target){
    return sqrt(pow(A->x - target->x,2) + pow(A->y - target->y,2));
}
bool Contains(Lista *lista, Node *adat){
    ListaElem y;
    y.kovi = lista ->elso;
    while( y.kovi!= NULL){
        if(y.kovi->adat.x == adat->x && y.kovi->adat.y == adat->y){
            return true;
        }
        y.kovi = y.kovi->kovi;
    }
    return false;
}
void retrace(Lista *path, Node *start, Node *end){
    Node seged = *end;
    while(seged.parent != start){
        hozzafuz(path, &seged);
        seged = *seged.parent;
    }
    hozzafuz(path, start);
}
void Pathfinder(Node *startNode, Node *targetNode, Lista openSet, Lista closedSet){
    ListaElem *currentP;
    currentP = hozzafuzSpec(&openSet, startNode, currentP);
    printf("%s", Contains(&openSet, startNode)? "igen" :"nem");
    while(openSet.elso != NULL && openSet.utolso != NULL){
        Node currentNode = openSet.elso->adat;
        currentP = openSet.elso;
        ListaElem seged;
        seged.kovi = openSet.elso;
        while(seged.kovi != NULL){
            if(fCost(&seged.kovi->adat) < fCost(&currentNode) || fCost(&seged.kovi->adat) == fCost(&currentNode) && seged.kovi->adat.hCost < currentNode.hCost){
                currentNode = seged.kovi->adat;
                currentP = seged.kovi;
            }
            seged.kovi = seged.kovi->kovi;
        }
        letorol(&openSet, currentP);
        currentP = hozzafuzSpec(&closedSet, &currentNode, currentP);

        printf("%f %f\n", currentNode.x, currentNode.y);
        if(currentNode.x == targetNode->x && currentNode.y == targetNode->y){
            return;
            //cél elérve : return
        }
        for (int i = 0; i < currentNode.fokszam; ++i) {
            Node *szomszed = currentNode.p[i].p1;
            if(Contains(&closedSet, szomszed)){
                continue;
            }
            double newMovementCost = currentNode.gCost + currentNode.p[i].hossz;
            if(newMovementCost < szomszed->gCost || !Contains(&openSet, szomszed)){
                szomszed->gCost = newMovementCost;
                szomszed->hCost = hCost(szomszed, targetNode);
                szomszed->parent = &currentNode;

                if(!Contains(&openSet, szomszed)){
                    hozzafuz(&openSet, szomszed);
                }
            }
        }
    }
}

int main() {
    Node a = {1,1,2, 0, sqrt(58), NULL, malloc(2*sizeof(Edge))};
    Node c = {4,4,3, INT_MAX, INT_MAX, NULL,malloc(3*sizeof(Edge))};
    Node d = {4,1,3, INT_MAX, INT_MAX, NULL, malloc(3*sizeof(Edge))};
    Node e = {8,4,2, INT_MAX, INT_MAX, NULL, malloc(2*sizeof(Edge))};

    a.p[0].hossz = 2;
    a.p[1].hossz = 6;
    a.p[0].p1 = &d;
    a.p[1].p1 = &c;

    c.p[0].hossz = 6;
    c.p[1].hossz = 1;
    c.p[2].hossz = 1;
    c.p[0].p1 = &a;
    c.p[1].p1 = &d;
    c.p[2].p1 = &e;

    d.p[0].hossz = 10;
    d.p[1].hossz = 1;
    d.p[2].hossz = 2;
    d.p[0].p1 = &e;
    d.p[1].p1 = &c;
    d.p[2].p1 = &a;

    e.p[0].hossz = 1;
    e.p[1].hossz = 5;
    e.p[0].p1 = &c;
    e.p[1].p1 = &d;

    Lista openSet = {NULL, NULL};
    Lista closedSet = {NULL, NULL};
    Lista path = {NULL, NULL};
    Pathfinder(&a, &e, openSet, closedSet);

    kiurit(&path);
    kiurit(&openSet);
    kiurit(&closedSet);
    free(a.p);
    free(c.p);
    free(d.p);
    free(e.p);
    return 0;
}
