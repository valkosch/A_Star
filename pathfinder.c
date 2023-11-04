
#include "pathfinder.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void retrace(List *path, Node *start, Node *end){
    Node seged = *end;
    while(seged.parent != NULL){
        addToList(path, &seged);
        seged = *seged.parent;
    }
    addToList(path, start);
}
void Pathfinder(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb){
    ListCell *currentP;
    Node currentNode;
    currentP = addToListSpec(openSet, startNode, currentP);
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
        currentP = addToListSpec(closedSet, &currentNode, currentP);
        printf("%f %f\n", currentNode.x, currentNode.y);
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
void SetStart(Node *startNode, Node *targetNode){
    startNode->gCost = 0;
    startNode ->hCost = hCost(startNode, targetNode);
}