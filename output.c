#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <math.h>
#include <SDL_ttf.h>
#include "graphElements.h"
#include "beolvasas.h"
#include "output.h"


void retraceDraw(Node *start, Node *end, SDL_Renderer *renderer){
    Node seged = *end;
    while(seged.parent != NULL){
        thickLineRGBA(renderer, seged.x, seged.y, seged.parent->x, seged.parent->y, 5, 0, 0, 255, 255);
        seged = *seged.parent;
    }
}
void RenderButtonWText(SDL_Renderer *renderer, TTF_Font *font, Button bt){
    roundedBoxRGBA(renderer, bt.to.x-2, bt.to.y-2, bt.to.x + bt.to.w+2, bt.to.y + bt.to.h+2,10 , 0, 0, 0 , 255);
    roundedBoxRGBA(renderer, bt.to.x, bt.to.y, bt.to.x + bt.to.w, bt.to.y + bt.to.h,10 , bt.Bcolor.r, bt.Bcolor.g, bt.Bcolor.b, 255);

    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };
    felirat = TTF_RenderUTF8_Solid(font, bt.str, bt.Tcolor);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = bt.to.x + bt.to.w/3;
    hova.y = bt.to.y + bt.to.h/6;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}
/* ablak letrehozasa */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}
void RenderMap(SDL_Renderer *renderer){
    SDL_Texture *map = IMG_LoadTexture(renderer, "datas/map.png");
        if (map == NULL) {
            SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
            SDL_DestroyTexture(map);
            exit(1);
        }
    SDL_Rect dest = { 0, 0, GetMapI().imgWidth, GetMapI().imgHeight };
    SDL_RenderCopy(renderer, map, NULL, &dest);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(map);
}
