
#include "pathfinder.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void PathfinderF(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb){
    ListCell *currentP;
    Node currentNode;
    addToListSpec(openSet, startNode, &currentP);
    while(openSet->first != NULL && openSet->last != NULL){
        currentNode = openSet->first->data;
        currentP = openSet->first;
        ListCell seged;
        seged.next = openSet->first;
        while(seged.next != NULL){
            if(fCost(&seged.next->data) < fCost(&currentNode) || fCost(&seged.next->data) == fCost(&currentNode) && seged.next->data.hCost < currentNode.hCost){
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
            double newMovementCost = currentNode.gCost + currentNode.p[i].time;
            if(newMovementCost < szomszed->gCost || !Contains(openSet, szomszed)){
                szomszed->gCost = newMovementCost;
                szomszed->hCost = hCost(szomszed, targetNode)/130;
                szomszed->parent = &tomb[currentNode.index];

                if(!Contains(openSet, szomszed)){
                    addToList(openSet, szomszed);
                }
            }
        }
    }
}
void PathfinderS(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb){
    ListCell *currentP;
    Node currentNode;
    addToListSpec(openSet, startNode, &currentP);
    while(openSet->first != NULL && openSet->last != NULL){
        currentNode = openSet->first->data;
        currentP = openSet->first;
        ListCell seged;
        seged.next = openSet->first;
        while(seged.next != NULL){
            if(fCost(&seged.next->data) < fCost(&currentNode) || fCost(&seged.next->data) == fCost(&currentNode) && seged.next->data.hCost < currentNode.hCost){
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
            double newMovementCost = currentNode.gCost + currentNode.p[i].length;
            if(newMovementCost < szomszed->gCost || !Contains(openSet, szomszed)){
                szomszed->gCost = newMovementCost;
                szomszed->hCost = hCost(szomszed, targetNode);
                szomszed->parent = &tomb[currentNode.index];

                if(!Contains(openSet, szomszed)){
                    addToList(openSet, szomszed);
                }
            }
        }
    }
}
void SetStartS(Node *startNode, Node *targetNode){
    startNode->gCost = 0;
    startNode ->hCost = hCost(startNode, targetNode);
}
void SetStartF(Node *startNode, Node *targetNode){
    startNode->gCost = 0;
    startNode ->hCost = hCost(startNode, targetNode)/130;
}