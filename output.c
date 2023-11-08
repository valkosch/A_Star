#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <math.h>
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
SDL_Texture *RenderMap(SDL_Renderer *renderer){
    SDL_Texture *map = IMG_LoadTexture(renderer, "C:\\Users\\Lenovo\\Documents\\iskola\\Astar_terkep\\datas\\terkep.png");
        if (map == NULL) {
            SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
            exit(1);
        }
    SDL_Rect dest = { 0, 0, GetMapI().imgWidth, GetMapI().imgHeight };
    SDL_RenderCopy(renderer, map, NULL, &dest);
    return map;
}
