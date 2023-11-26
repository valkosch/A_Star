
#include "pathfinder.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void FindNearest_Node(int clickX, int clickY, NodeTomb *mainArray, Node **B){ /*megkeresi azt a pontot ami a legközelebb van az adott x és y koordinátákhoz*/
    Node *res;
    double min = sqrt(pow(clickX-mainArray->nodes[0].x,2) + pow(clickY-mainArray->nodes[0].y,2));
    for (int i = 0; i < mainArray->NodeNum; i++) /*nem a legoptimálisabb de a lista struktúra csak ezt a minimum keresést teszi lehetőve, jövőben javítani kell*/
    {
        double tmp = sqrt(pow(clickX-mainArray->nodes[i].x,2) + pow(clickY-mainArray->nodes[i].y,2));
        if(min > tmp){
            res = &mainArray->nodes[i];
            min = tmp;
        }
    }
    *B = res;
}
void Pathfinder(Node *startNode, Node *targetNode, List *openSet, List *closedSet, Node *tomb, ButtonType type){ /*A* algoritmus gyakorlati megvalósítása*/
    ListCell *currentP;
    Node currentNode;
    addToListSpec(openSet, startNode, &currentP);
    while(openSet->first != NULL && openSet->last != NULL){ /*amig az openSetben van elem, tehát amíg van pont ami még nem volt kivizsgálva*/
        currentNode = openSet->first->data; /*a kiindulásnál ez a kezdőpont ami kiválasztott a térképen a felhasználó*/
        currentP = openSet->first;
        ListCell seged;
        seged.next = openSet->first;
        while(seged.next != NULL){ /*megnezzük hogy a jelenlegi kiválasztottnál van e jobb jelölt az opensetben, (ez a keresés sem a legoptimálisabb)*/
            if((fCost(&seged.next->data) < fCost(&currentNode)) || (fCost(&seged.next->data) == fCost(&currentNode) && seged.next->data.hCost < currentNode.hCost)){
                currentNode = seged.next->data; /*ha van akkor legyen ő az új kiválasztott*/
                currentP = seged.next;
            }
            seged.next = seged.next->next;
        }
        removeFromList(openSet, currentP); /*kivizsgáltuk a kiválasztottat, így már legközelebb nem kell megnézni, ezért kitöröljök a listából*/
        addToListSpec(closedSet, &currentNode, &currentP); /*hozzáadjuk a kivizsgáltakhoz*/
        if(currentNode.x == targetNode->x && currentNode.y == targetNode->y){ /*ha a kiválasztott épp a cél*/
            return;
            //cél elérve : return
        }
        for (int i = 0; i < currentNode.fokszam; ++i) { /*nézzük meg a kiválasztott összes szomszédját (pontot ami elérhető a kiválasztottból közvetlenül éllel)*/
            Node *szomszed = currentNode.p[i].p1;
            if(Contains(closedSet, szomszed)){
                continue;
            }
            double newMovementCost;
            switch (type) /*attól függ hogy melyik módban indítottuk el, hogy mit tekintünk súlynak*/
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
            if(newMovementCost < szomszed->gCost || !Contains(openSet, szomszed)){ /*mindenszomszédnál beállítjuk az új gCost-t és hCost-ot ha a mostani kiválasztottan keresztül rövidebb utat találtunk a szomszédhoz*/
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

                if(!Contains(openSet, szomszed)){  /*szomszédot hozzáadjuk a mégkivizsgálandókhoz*/
                    addToList(openSet, szomszed);
                }
            }
        }
    }
}
void SetStart(Node *startNode, Node *targetNode, ButtonType type){ /*beállítja a kezdőpontot és végpontot a szükséges kezdőértékekkel*/
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