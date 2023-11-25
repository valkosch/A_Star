#ifndef STATES_H
#define STATES_H

#include "pathfinder.h"
#include "beolvasas.h"
#include "output.h"

extern SDL_Color red;
extern SDL_Color blue;
extern SDL_Color black;
extern SDL_Color white;
extern SDL_Color grey;
extern SDL_Color purple;

bool CheckButtonClick(Button *bt, SDL_Event *ev);
void ButtonEffect(SDL_Renderer *renderer, Button *bt, TTF_Font *font);
void SelectNode(SDL_Renderer *renderer,SDL_Event *event, Node **StartNode, Vector2 *tmp, NodeTomb *mainArray);
void InitPathfinding(NodeTomb *mainArray, List *openSet, List *closedSet);
#endif