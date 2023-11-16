#ifndef STATES_H
#define STATES_H

#include "pathfinder.h"
#include "beolvasas.h"
#include "output.h"

typedef struct StateParams{
    Node *StartNode, *EndNode, *mainArray;
    List *openSet, *closedSet;
    SDL_Renderer *renderer;
    SDL_Event event;
    int nodeNum;
}StateParams;

#endif