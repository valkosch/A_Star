
#include "pathfinder.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void FindNearest_Node(int clickX, int clickY, NodeTomb *mainArray, Node **A){
    Node *res;
    double min = sqrt(pow(clickX-mainArray->nodes[0].x,2) + pow(clickY-mainArray->nodes[0].y,2));
    for (int i = 0; i < mainArray->NodeNum; i++)
    {
        double tmp = sqrt(pow(clickX-mainArray->nodes[i].x,2) + pow(clickY-mainArray->nodes[i].y,2));
        if(min > tmp){
            res = &mainArray->nodes[i];
            min = tmp;
        }
    }
    *A = res;
}
void Pathfinder(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb, ButtonType type){
    ListCell *currentP;
    Node currentNode;
    addToListSpec(openSet, startNode, &currentP);
    while(openSet->first != NULL && openSet->last != NULL){
        currentNode = openSet->first->data;
        currentP = openSet->first;
        ListCell seged;
        seged.next = openSet->first;
        while(seged.next != NULL){
            if((fCost(&seged.next->data) < fCost(&currentNode)) || (fCost(&seged.next->data) == fCost(&currentNode) && seged.next->data.hCost < currentNode.hCost)){
                currentNode = seged.next->data;
                currentP = seged.next;
            }
            seged.next = seged.next->next;
        }
        removeFromList(openSet, currentP);
        addToListSpec(closedSet, &currentNode, &currentP);
        if(currentNode.x == targetNode->x && currentNode.y == targetNode->y){
            return;
            //cél elérve : return
        }
        for (int i = 0; i < currentNode.fokszam; ++i) {
            Node *szomszed = currentNode.p[i].p1;
            if(Contains(closedSet, szomszed)){
                continue;
            }
            double newMovementCost;
            switch (type)
            {
                case FASTEST:
                    newMovementCost = currentNode.gCost + currentNode.p[i].time;
                    break;
                case SHORTEST:
                    newMovementCost = currentNode.gCost + currentNode.p[i].length;
                    break;
                default:
                    printf("Hiba, roszz típus");
                    break;
            }
            if(newMovementCost < szomszed->gCost || !Contains(openSet, szomszed)){
                szomszed->gCost = newMovementCost;
                switch (type)
                {
                    case FASTEST:
                        szomszed->hCost = hCost(szomszed, targetNode)/50;
                        break;
                    case SHORTEST:
                        szomszed->hCost = hCost(szomszed, targetNode);
                        break;
                    default:
                        printf("Hiba, roszz típus");
                        break;
                }
                szomszed->parent = &tomb[currentNode.index];

                if(!Contains(openSet, szomszed)){
                    addToList(openSet, szomszed);
                }
            }
        }
    }
}
void SetStart(Node *startNode, Node *targetNode, ButtonType type){
    switch (type)
    {
    case SHORTEST:
        startNode->gCost = 0;
        startNode ->hCost = hCost(startNode, targetNode);
        break;
    case FASTEST:
        startNode->gCost = 0;
        startNode ->hCost = hCost(startNode, targetNode)/50;
        break;
    default:
        printf("Hiba, roszz típus");
        break;
    }
}