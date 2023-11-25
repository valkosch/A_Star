#include "states.h"

SDL_Color red = {.r= 255, .g = 0, .b=0, .a=255};
SDL_Color blue = {.r= 0, .g = 0, .b=255, .a=255};
SDL_Color black = {.r= 0, .g = 0, .b=0, .a=255};
SDL_Color white = {.r= 255, .g = 255, .b=255, .a=255};
SDL_Color grey = {.r = 211, .g = 211, .b = 211, .a=255};
SDL_Color purple = {.r = 204, .g = 0, .b = 204, .a=255};

bool CheckButtonClick(Button *bt, SDL_Event *ev){
    return ((ev->button.x >= bt->to.x && ev->button.x <= (bt->to.x+bt->to.w))&&(ev->button.y >= bt->to.y && ev->button.y <= (bt->to.y+bt->to.h)));
}
void ButtonEffect(SDL_Renderer *renderer, Button *bt, TTF_Font *font){
    SDL_Color tmp;
    switch (bt->t)
    {
        case SHORTEST:
            tmp = blue;
            break;
        
        case FASTEST:
            tmp = red;
            break;
        default:
            break;
    }
    if(bt->power){
        bt->Bcolor = grey;
        bt->Tcolor = black;
        RenderButtonWText(renderer,font, *bt);
        SDL_RenderPresent(renderer);
        bt->power = false;
    }
    else{
        bt->Bcolor = tmp;
        bt->Tcolor = white;
        RenderButtonWText(renderer,font, *bt);
        SDL_RenderPresent(renderer);
        bt->power = true;
    }
}
void IconEffect(SDL_Renderer *renderer, Button bt, mapType *map){
    if(*map == SAT){
        RenderMap(renderer, NORM);
        RenderButtonIcon(renderer, bt, SAT);
        *map = NORM;
    }
    else{
        RenderMap(renderer, SAT);
        RenderButtonIcon(renderer, bt, NORM);
        *map = SAT;
    }
}
void SelectNode(SDL_Renderer *renderer,SDL_Event *event, Node **StartNode, Vector2 *tmp, NodeTomb *mainArray){
    FindNearest_Node(event->button.x,event->button.y,mainArray, StartNode);
    filledCircleRGBA(renderer, event->button.x, event->button.y, 4, 255, 0, 0, 255);
    tmp->x = event->button.x;
    tmp->y = event->button.y;
    SDL_RenderPresent(renderer);   
}
void InitPathfinding(NodeTomb *mainArray, List *openSet, List *closedSet){
    for(int i = 0; i< mainArray->NodeNum; i++){
        NodeInit(&(mainArray->nodes[i]));
    }
    DeleteList(openSet);
    DeleteList(closedSet);
}