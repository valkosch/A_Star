#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "debugmalloc.h"
#include "states.h"

/* a main vezérli az adatstruktúrát (inicializálja,feltölti,továbbadja,törli)*/
int main(int argc, char *argv[]) {
    /*gráf pontok és élek tömbjeihez tartozó függvények meghívása*/
    NodeTomb mainArray;
    NodeInput(&mainArray);
    EdgeInput(&mainArray);
    mapItNodes(&mainArray);

    
    mapInfo mapI = GetMapI(); /*térkép adottságait eltároló var*/
    List openSet = {NULL, NULL}; /*A* müködéséhez fontos 2 lista*/
    List closedSet = {NULL, NULL};
    Node *StartNode = NULL;/*kezdő és végpont(útkereszteződés) amik között keressük az útvonalat*/
    Node *EndNode = NULL;
    Vector2 StartClick;/*felhasználó kattintásainak helyei a képernyőn*/
    Vector2 EndClick;
    mapType map = NORM; /*enumok az állapotok egyszerűbb kezeléséért*/
    ClickState State = FROM;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Button buttons[4] = {
        {.to = {.x = mapI.imgWidth +  10, .y = 200, .w = 235, .h = 60}, .Bcolor = grey, .Tcolor = black, .str = "Leggyorsabb út", .t=FASTEST, .power =false},
        {.to = {.x = mapI.imgWidth +  260, .y = 200, .w = 230, .h = 60}, .Bcolor = grey, .Tcolor = black, .str = "Legrövidebb út", .t= SHORTEST, .power =false},
        {.to = {.x = mapI.imgWidth +  275, .y = mapI.imgHeight-85, .w = 200, .h = 60}, .Bcolor = purple, .Tcolor = white, .str = "GO-->>>", .t=GO, .power =false},
        {.to = {.x = mapI.imgWidth -90, .y = mapI.imgHeight-90, .w = 80, .h = 80}, .Bcolor = white, .Tcolor = white, .str = "", .t=ICON, .power =false}
    };

    /*ablak inicializálása*/
    sdl_init("Pathfinder", mapI.imgWidth+500, mapI.imgHeight, &window, &renderer);
    RenderMap(renderer, map);
    boxRGBA(renderer, mapI.imgWidth, 0, mapI.imgWidth + 500, mapI.imgHeight,  255, 255, 255, 255);
    RenderLogo(renderer);

    /*font betöltése és statikus szöveg kiírása*/
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

    /*gombok renderelése*/
    for (int i = 0; i < 3; i++)
    {
        RenderButtonWText(renderer,font, buttons[i]);
    }

    RenderButtonIcon(renderer, buttons[3], map);

    /*fő eseményvezérlés*/
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
            /* egergomb elengedese */
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_RIGHT) { /*térkép állapotának resetje, kezdőállapotba helyezés jobb klikk felengedés hatására*/
                    State = FROM;
                    buttons[GO].power = false;
                    RenderMap(renderer, map);
                    RenderButtonIcon(renderer, buttons[3], map);
                    boxRGBA(renderer, mapI.imgWidth, 310, mapI.imgWidth + 500, mapI.imgHeight-90,  255, 255, 255, 255);
                    SDL_RenderPresent(renderer);
                    InitPathfinding(&mainArray, &openSet, &closedSet);
                }
                if (event.button.button == SDL_BUTTON_LEFT) { 
                    if(event.button.x <= mapI.imgWidth && !CheckButtonClick(&buttons[ICON], &event)){ /*térképen bal klikk felengedés*/
                        switch (State)
                        {
                        case FROM:
                            SelectNode(renderer, &event,&StartNode, &StartClick, &mainArray); /*kezdőpont kiválasztása*/
                            State = TO;
                            break;
                        case TO:
                            SelectNode(renderer, &event,&EndNode, &EndClick, &mainArray); /*végpont kiválasztása*/
                            State = WAIT;
                            break;
                        default:
                            break;
                        }
                    }
                    else if(CheckButtonClick(&buttons[SHORTEST], &event)){ /*"Legrövidebb út" bomb megnyomása*/
                        ButtonEffect(renderer, &buttons[SHORTEST], font);
                    }
                    else if(CheckButtonClick(&buttons[FASTEST], &event)){ /*"Leggyorsabb út" bomb megnyomása*/
                        ButtonEffect(renderer, &buttons[FASTEST], font);
                    }
                    else if(CheckButtonClick(&buttons[GO], &event)&& State == WAIT){ /*"GO" bomb megnyomása*/
                        RenderMap(renderer, map);
                        boxRGBA(renderer, mapI.imgWidth, 310, mapI.imgWidth + 500, mapI.imgHeight-90,  255, 255, 255, 255);
                        filledCircleRGBA(renderer, StartClick.x, StartClick.y, 4, 255, 0, 0, 255);
                        filledCircleRGBA(renderer, EndClick.x, EndClick.y, 4, 255, 0, 0, 255);
                        RenderButtonIcon(renderer, buttons[3], map);
                        /*A gombbal kiválasztott (nem kizáró vagy) út kirajzolása */
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
                    else if(CheckButtonClick(&buttons[ICON], &event)){ /*térkép nézet váltása (SAT = műholdas kép, NORM = sima térkép)*/
                        if(map == SAT){
                            map = NORM;
                        }
                        else{
                            map = SAT;
                        }
                        RenderMap(renderer, map);
                        RenderButtonIcon(renderer, buttons[3], map);
                        switch (State) /*hogy ne vesszenek el az eddig megjelenített pontok vagy útvonal újra kirajzoljuk állapottól függően*/
                        {
                            case TO: /*ha csak 1 pont volt eddig kiválasztva*/
                                filledCircleRGBA(renderer, StartClick.x, StartClick.y, 4, 255, 0, 0, 255);
                                SDL_RenderPresent(renderer);
                                break;
                            case WAIT: /*ha már mindkettő pont ki volt választva*/
                                filledCircleRGBA(renderer, StartClick.x, StartClick.y, 4, 255, 0, 0, 255);
                                filledCircleRGBA(renderer, EndClick.x, EndClick.y, 4, 255, 0, 0, 255);
                                SDL_RenderPresent(renderer);
                                if(buttons[GO].power){ /*ha már az útvonal(ak) is ki volt(ak) rajzolva*/
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
    FreeNodes(&mainArray); /*felszabadítások*/
    DeleteList(&openSet);
    DeleteList(&closedSet);
    TTF_CloseFont(font);
    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
