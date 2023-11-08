#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include "graphElements.h"

void retraceDraw(Node *start, Node *end, SDL_Renderer *renderer);
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
SDL_Texture *RenderMap(SDL_Renderer *renderer);
#endif // OUTPUT_H_INCLUDED
