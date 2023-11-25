#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "graphElements.h"
typedef enum mapType{
        NORM = 0,
        SAT = 1
}mapType;
typedef enum ClickState{
        FROM = 0,
        TO = 1,
        WAIT = 2
}ClickState;
typedef enum ButtonType{
        FASTEST = 0,
        SHORTEST = 1,
        GO = 2,
        ICON = 3
}ButtonType;
typedef struct Button{
        SDL_Rect to;
        char *str;
        SDL_Color Bcolor;
        SDL_Color Tcolor;
        ButtonType t;
        bool power;
}Button;
typedef struct Text{
        SDL_Rect to;
        char *str;
        SDL_Color color;
        TTF_Font *font;
}Text;
void RenderText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, SDL_Rect *hova, char *str);
void RenderButtonWText(SDL_Renderer *renderer, TTF_Font *font, Button bt);
void RenderLogo(SDL_Renderer *renderer);
void RenderButtonIcon(SDL_Renderer *renderer, Button bt, mapType a);
void retraceDraw(Node *start, Node *end, SDL_Renderer *renderer, SDL_Color color, TTF_Font *font);
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void RenderMap(SDL_Renderer *renderer, mapType a);

#endif // OUTPUT_H_INCLUDED
