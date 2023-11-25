#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <math.h>
#include <SDL_ttf.h>
#include "graphElements.h"
#include "beolvasas.h"
#include "output.h"


void RenderText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, SDL_Rect *hova, char *str){
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;

    felirat = TTF_RenderUTF8_Solid(font, str, color);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);

    hova->w = felirat->w;
    hova->h = felirat->h;

    SDL_RenderCopy(renderer, felirat_t, NULL, hova);
    SDL_RenderPresent(renderer);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}
void timeofJourney(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, double timeSec){
    SDL_Rect hova = { 0, 0, 0, 0 };
    char str[15];
    int perc = timeSec / 60;
    int mp = (int)timeSec % 60;
    sprintf(str, "%d perc %d mp", perc, mp);
    mapInfo mapI = GetMapI();
    switch (color.r)
    {
    case 255:
        hova.x = mapI.imgWidth + 10;
        hova.y = 310;
        break;

    default:
        hova.x = mapI.imgWidth + 260;
        hova.y = 310;
        break;
    }
    RenderText(renderer, font, color, &hova, str);
}
void lengthofJourney(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, double length){
    SDL_Rect hova = { 0, 0, 0, 0 };
    char str[15];
    int km = length / 1000;
    int m = (int)length % 1000;
    sprintf(str, "%dkm %dm", km, m);
    mapInfo mapI = GetMapI();
    switch (color.r)
    {
    case 255:
        hova.x = mapI.imgWidth + 10;
        hova.y = 360;
        break;

    default:
        hova.x = mapI.imgWidth + 260;
        hova.y = 360;
        break;
    }
    RenderText(renderer, font, color, &hova, str);
}
void retraceDraw(Node *start, Node *end, SDL_Renderer *renderer, SDL_Color color, TTF_Font *font){
    Node seged = *end;
    double lengthSum = 0;
    double timeSum = 0;
    while(seged.parent != NULL){
        for (int i = 0; i < seged.fokszam; i++)
        {
            if(seged.parent == seged.p[i].p1){
                lengthSum += seged.p[i].length;
                timeSum += seged.p[i].time;
                Vector2Cell *iter = seged.p[i].linestring;
                while(iter->next != NULL){
                    Vector2Cell *mozgo = iter->next;
                    thickLineRGBA(renderer, iter->data.x, iter->data.y, mozgo->data.x, mozgo->data.y, 5, color.r, color.g, color.b, color.a);
                    iter = mozgo;
                }
                break;
            }
        }

        seged = *seged.parent;
    }
    lengthofJourney(renderer, font, color, lengthSum);
    timeofJourney(renderer, font, color, timeSum);
    SDL_RenderPresent(renderer);
}
void RenderButtonWText(SDL_Renderer *renderer, TTF_Font *font, Button bt){
    roundedBoxRGBA(renderer, bt.to.x-2, bt.to.y-2, bt.to.x + bt.to.w+2, bt.to.y + bt.to.h+2,10 , 0, 0, 0 , 255);
    roundedBoxRGBA(renderer, bt.to.x, bt.to.y, bt.to.x + bt.to.w, bt.to.y + bt.to.h,10 , bt.Bcolor.r, bt.Bcolor.g, bt.Bcolor.b, bt.Bcolor.a);


    SDL_Rect hova = { 0, 0, 0, 0 };
    hova.x = bt.to.x + bt.to.w/20;
    hova.y = bt.to.y + bt.to.h/6;
    RenderText(renderer, font, bt.Tcolor, &hova, bt.str);
}

void RenderButtonIcon(SDL_Renderer *renderer, Button bt, mapType a){
    SDL_Texture *icon = NULL;
    switch (a)
    {
        case NORM:
            icon = IMG_LoadTexture(renderer, "sat_icon.png");
            break;
        case SAT:
            icon = IMG_LoadTexture(renderer, "map_icon.png");
            break;
    }
    if (icon == NULL) {
            SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
            SDL_DestroyTexture(icon);
            exit(1);
    }
    filledCircleRGBA(renderer, bt.to.x + bt.to.w/2, bt.to.y + bt.to.h/2, 43, 0, 0, 0, 255);
    SDL_RenderCopy(renderer, icon, NULL, &bt.to);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(icon);
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
void RenderMap(SDL_Renderer *renderer, mapType a){
    SDL_Texture *map = NULL;
    switch (a)
    {
        case NORM:
            map = IMG_LoadTexture(renderer, "map.png");
            break;
        case SAT:
            map = IMG_LoadTexture(renderer, "sat.png");
            break;
    }
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
void RenderLogo(SDL_Renderer *renderer){
    SDL_Texture *img = IMG_LoadTexture(renderer, "logo1.png");
     if (img == NULL) {
            SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
            SDL_DestroyTexture(img);
            exit(1);
    }
    SDL_Rect dest = { GetMapI().imgWidth, 0, 500, 200 };
    SDL_RenderCopy(renderer, img, NULL, &dest);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(img);
}
