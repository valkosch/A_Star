#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"
#include "states.h"


int main(int argc, char *argv[]) {
    NodeTomb mainArray;
    NodeInput(&mainArray);
    EdgeInput(&mainArray);
    mapItNodes(&mainArray);

    mapInfo mapI = GetMapI();
    List openSet = {NULL, NULL};
    List closedSet = {NULL, NULL};
    Node *StartNode = NULL;
    Node *EndNode = NULL;
    Vector2 StartClick;
    Vector2 EndClick;
    mapType map = NORM;
    ClickState State = FROM;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Button buttons[4] = {
        {.to = {.x = mapI.imgWidth +  10, .y = 200, .w = 235, .h = 60}, .Bcolor = grey, .Tcolor = black, .str = "Leggyorsabb út", .t=FASTEST, .power =false},
        {.to = {.x = mapI.imgWidth +  260, .y = 200, .w = 230, .h = 60}, .Bcolor = grey, .Tcolor = black, .str = "Legrövidebb út", .t= SHORTEST, .power =false},
        {.to = {.x = mapI.imgWidth +  275, .y = mapI.imgHeight-85, .w = 200, .h = 60}, .Bcolor = purple, .Tcolor = white, .str = "GO-->>>", .t=GO, .power =false},
        {.to = {.x = mapI.imgWidth -90, .y = mapI.imgHeight-90, .w = 80, .h = 80}, .Bcolor = white, .Tcolor = white, .str = "", .t=ICON, .power =false}
    };


    sdl_init("Pathfinder", mapI.imgWidth+500, mapI.imgHeight, &window, &renderer);
    RenderMap(renderer, map);
    boxRGBA(renderer, mapI.imgWidth, 0, mapI.imgWidth + 500, mapI.imgHeight,  255, 255, 255, 255);
    RenderLogo(renderer);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("ProductSans-Regular.ttf", 25);
    Text info = {.to = {.x = mapI.imgWidth +  10, .y = 270, .w = 0, .h = 0}, .color = black, .str = "Forgalom mentesen az út hossza és ideje:"};
    RenderText(renderer, font, info.color,&info.to,info.str );
    TTF_CloseFont(font);

    font = TTF_OpenFont("ProductSans-Regular.ttf", 32);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    for (int i = 0; i < 3; i++)
    {
        RenderButtonWText(renderer,font, buttons[i]);
    }

    RenderButtonIcon(renderer, buttons[3], map);
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
            /* egergomb elengedese */
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_RIGHT) {
                    State = FROM;
                    RenderMap(renderer, map);
                    RenderButtonIcon(renderer, buttons[3], map);
                    boxRGBA(renderer, mapI.imgWidth, 310, mapI.imgWidth + 500, mapI.imgHeight-90,  255, 255, 255, 255);
                    SDL_RenderPresent(renderer);
                    InitPathfinding(&mainArray, &openSet, &closedSet);
                }
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if(event.button.x <= mapI.imgWidth && !CheckButtonClick(&buttons[ICON], &event)){
                        switch (State)
                        {
                        case FROM:
                            SelectNode(renderer, &event,&StartNode, &StartClick, &mainArray);
                            State = TO;
                            break;
                        case TO:
                            SelectNode(renderer, &event,&EndNode, &EndClick, &mainArray);
                            State = WAIT;
                            break;
                        default:
                            break;
                        }
                    }
                    else if(CheckButtonClick(&buttons[SHORTEST], &event)){
                        ButtonEffect(renderer, &buttons[SHORTEST], font);
                    }
                    else if(CheckButtonClick(&buttons[FASTEST], &event)){
                        ButtonEffect(renderer, &buttons[FASTEST], font);
                    }
                    else if(CheckButtonClick(&buttons[GO], &event)&& State == WAIT){
                        RenderMap(renderer, map);
                        boxRGBA(renderer, mapI.imgWidth, 310, mapI.imgWidth + 500, mapI.imgHeight-90,  255, 255, 255, 255);
                        filledCircleRGBA(renderer, StartClick.x, StartClick.y, 4, 255, 0, 0, 255);
                        filledCircleRGBA(renderer, EndClick.x, EndClick.y, 4, 255, 0, 0, 255);
                        RenderButtonIcon(renderer, buttons[3], map);

                        if(buttons[SHORTEST].power){
                            SetStart(StartNode, EndNode, SHORTEST);
                            Pathfinder(StartNode, EndNode, &openSet, &closedSet, mainArray.nodes, SHORTEST);
                            retraceDraw(StartNode, EndNode, renderer ,blue, font);
                            InitPathfinding(&mainArray, &openSet, &closedSet);
                        }
                        if(buttons[FASTEST].power){
                            SetStart(StartNode, EndNode, FASTEST);
                            Pathfinder(StartNode, EndNode, &openSet, &closedSet, mainArray.nodes, FASTEST);
                            retraceDraw(StartNode, EndNode, renderer, red, font);
                            InitPathfinding(&mainArray, &openSet, &closedSet);
                        }
                        if(buttons[FASTEST].power || buttons[SHORTEST].power){
                            thickLineRGBA(renderer, EndNode->x, EndNode->y, EndClick.x, EndClick.y, 5, 0, 0, 0, 255);
                            thickLineRGBA(renderer, StartNode->x, StartNode->y, StartClick.x, StartClick.y, 5, 0, 0, 0, 255);
                            filledCircleRGBA(renderer, StartNode->x, StartNode->y, 4, 255, 0, 0, 255);
                            filledCircleRGBA(renderer, EndNode->x, EndNode->y, 4, 255, 0, 0, 255);
                            SDL_RenderPresent(renderer);
                        }
                        buttons[GO].power =true;
                        RenderButtonIcon(renderer, buttons[3], map);
                    }
                    else if(CheckButtonClick(&buttons[ICON], &event)){
                        if(map == SAT){
                            map = NORM;
                        }
                        else{
                            map = SAT;
                        }
                        RenderMap(renderer, map);
                        RenderButtonIcon(renderer, buttons[3], map);
                        switch (State)
                        {
                            case TO:
                                filledCircleRGBA(renderer, StartClick.x, StartClick.y, 4, 255, 0, 0, 255);
                                SDL_RenderPresent(renderer);
                                break;
                            case WAIT:
                                filledCircleRGBA(renderer, StartClick.x, StartClick.y, 4, 255, 0, 0, 255);
                                filledCircleRGBA(renderer, EndClick.x, EndClick.y, 4, 255, 0, 0, 255);
                                SDL_RenderPresent(renderer);
                                if(buttons[GO].power){
                                    if(buttons[SHORTEST].power){
                                    SetStart(StartNode, EndNode, SHORTEST);
                                    Pathfinder(StartNode, EndNode, &openSet, &closedSet, mainArray.nodes, SHORTEST);
                                    retraceDraw(StartNode, EndNode, renderer ,blue, font);
                                    InitPathfinding(&mainArray, &openSet, &closedSet);
                                    }
                                    if(buttons[FASTEST].power){
                                        SetStart(StartNode, EndNode, FASTEST);
                                        Pathfinder(StartNode, EndNode, &openSet, &closedSet, mainArray.nodes, FASTEST);
                                        retraceDraw(StartNode, EndNode, renderer, red, font);
                                        InitPathfinding(&mainArray, &openSet, &closedSet);
                                    }
                                    if(buttons[FASTEST].power || buttons[SHORTEST].power){
                                        thickLineRGBA(renderer, EndNode->x, EndNode->y, EndClick.x, EndClick.y, 5, 0, 0, 0, 255);
                                        thickLineRGBA(renderer, StartNode->x, StartNode->y, StartClick.x, StartClick.y, 5, 0, 0, 0, 255);
                                        filledCircleRGBA(renderer, StartNode->x, StartNode->y, 4, 255, 0, 0, 255);
                                        filledCircleRGBA(renderer, EndNode->x, EndNode->y, 4, 255, 0, 0, 255);
                                        SDL_RenderPresent(renderer);
                                    }
                                    RenderButtonIcon(renderer, buttons[3], map);
                                }
                                break;

                            default:
                                break;
                        }
                    }
                }
                break;
            /* ablak bezarasa */
            case SDL_QUIT:
                quit = true;
                break;
        }
    }
    FreeNodes(&mainArray);
    DeleteList(&openSet);
    DeleteList(&closedSet);
    TTF_CloseFont(font);
    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
