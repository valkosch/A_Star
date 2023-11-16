#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"
#include "pathfinder.h"
#include "beolvasas.h"
#include "output.h"

void FindNearest_Node(int clickX, int clickY, Node *mainArray, int nodeNum, Node **A){
    Node *res;
    double min = sqrt(pow(clickX-mainArray[0].x,2) + pow(clickY-mainArray[0].y,2));
    for (int i = 0; i < nodeNum; i++)
    {
        double tmp = sqrt(pow(clickX-mainArray[i].x,2) + pow(clickY-mainArray[i].y,2));
        if(min > tmp){
            res = &mainArray[i];
            min = tmp;
        }
    }
    *A = res;
}
int main(int argc, char *argv[]) {

    int nodeNum;
    Node *mainArray = NodeInput(&nodeNum);
    EdgeInput(mainArray, &nodeNum);
    mapItNodes(mainArray, &nodeNum);
    mapInfo mapI = GetMapI();

    List openSet = {NULL, NULL};
    List closedSet = {NULL, NULL};
    Node *StartNode;
    Node *EndNode;

    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    Button buttons[3] = {
        {.to = {.x = mapI.imgWidth +  25, .y = 100, .w = 200, .h = 60}, .Bcolor = {.r = 211, .g = 211, .b = 211}, .Tcolor = {.r = 0, .g = 0, .b = 0}, .str = "gyors"},
        {.to = {.x = mapI.imgWidth +  275, .y = 100, .w = 200, .h = 60}, .Bcolor = {.r = 211, .g = 211, .b = 211}, .Tcolor = {.r = 0, .g = 0, .b = 0}, .str = "rovid"},
        {.to = {.x = mapI.imgWidth +  275, .y = mapI.imgHeight-85, .w = 200, .h = 60}, .Bcolor = {.r = 255, .g = 0, .b = 0}, .Tcolor = {.r = 255, .g = 255, .b = 255}, .str = "GO"},
    };
    sdl_init("Pathfinder", mapI.imgWidth+500, mapI.imgHeight, &window, &renderer);
    RenderMap(renderer);
    boxRGBA(renderer, mapI.imgWidth, mapI.imgHeight, mapI.imgWidth + 500, 0,  255, 255, 255, 255);
    
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("datas/ProductSans-Regular.ttf", 32);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    for (int i = 0; i < 3; i++)
    {
        RenderButtonWText(renderer,font, buttons[i]);
    }
    
    bool quit = false;
    int x = 0;
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
            /* eger kattintas */
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= mapI.imgWidth) {
                    switch (x)
                    {
                    case 0:
                        FindNearest_Node(event.button.x,event.button.y,mainArray,nodeNum, &StartNode);
                        filledCircleRGBA(renderer, event.button.x, event.button.y, 4, 255, 0, 0, 255);
                        SDL_RenderPresent(renderer);
                        thickLineRGBA(renderer, StartNode->x, StartNode->y, event.button.x, event.button.y, 5, 0, 0, 255, 255);
                        break;
                    case 1:
                        FindNearest_Node(event.button.x,event.button.y,mainArray,nodeNum, &EndNode);
                        filledCircleRGBA(renderer, event.button.x, event.button.y, 4, 255, 0, 0, 255);
                        SDL_RenderPresent(renderer);
                        thickLineRGBA(renderer, EndNode->x, EndNode->y, event.button.x, event.button.y, 5, 0, 0, 255, 255);
                        break;
                    default:
                        break;
                    }
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    x = 0;
                    for(int i = 0; i< nodeNum; i++){
                        NodeInit(&mainArray[i]);
                    }
                    DeleteList(&openSet);
                    DeleteList(&closedSet);
                    RenderMap(renderer);
                }
                break;
            /* egergomb elengedese */
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && event.button.x <= mapI.imgWidth) {
                    switch (x)
                    {
                    case 0:
                        x++;
                        break;
                    case 1:
                        SetStartS(StartNode, EndNode);
                        PathfinderS(StartNode, EndNode, &openSet, &closedSet, mainArray);
                        retraceDraw(StartNode, EndNode, renderer);
                        filledCircleRGBA(renderer, StartNode->x, StartNode->y, 4, 255, 0, 0, 255);
                        filledCircleRGBA(renderer, EndNode->x, EndNode->y, 4, 255, 0, 0, 255);
                        SDL_RenderPresent(renderer);
                        x++;
                        break;
                    default:
                        break;
                    }
                }
                break;
            /* ablak bezarasa */
            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    printf("\nstart: %d, end: %d", StartNode->index, EndNode->index);
    FreeNodes(nodeNum, mainArray);
    DeleteList(&openSet);
    DeleteList(&closedSet);
    TTF_CloseFont(font);
    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
