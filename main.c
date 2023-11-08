#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"
#include "pathfinder.h"
#include "beolvasas.h"
#include "output.h"




int main(int argc, char *argv[]) {

    int nodeNum;
    Node *test = NodeInput(&nodeNum);
    EdgeInput(test, &nodeNum);
    mapItNodes(test, &nodeNum);

    List openSet = {NULL, NULL};
    List closedSet = {NULL, NULL};

    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Pathfinder", GetMapI().imgWidth, GetMapI().imgHeight, &window, &renderer);
    SDL_Texture *map = RenderMap(renderer);

    srand(time(NULL));
    int rnd0 = rand() % nodeNum;
    int rnd1 = rand() % nodeNum;
    SetStart(&test[rnd0], &test[rnd1]);
    Pathfinder(&test[rnd0], &test[rnd1], &openSet, &closedSet, test);
    retraceDraw(&test[rnd0], &test[rnd1], renderer);
    filledCircleRGBA(renderer, test[rnd0].x, test[rnd0].y, 4, 255, 0, 0, 255);
    filledCircleRGBA(renderer, test[rnd1].x, test[rnd1].y, 4, 255, 0, 0, 255);


    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(map);


    FreeNodes(nodeNum, test);
    DeleteList(&openSet);
    DeleteList(&closedSet);
    /* varunk a kilepesre */
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
    }

    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
