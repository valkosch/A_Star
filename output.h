#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "graphElements.h"
typedef struct Button{
        SDL_Rect to;
        char *str;
        SDL_Color Bcolor;
        SDL_Color Tcolor;
}Button;

void RenderButtonWText(SDL_Renderer *renderer, TTF_Font *font, Button bt);
void retraceDraw(Node *start, Node *end, SDL_Renderer *renderer);
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void RenderMap(SDL_Renderer *renderer);
#endif // OUTPUT_H_INCLUDED
