
#include "beolvasas.h"
#include "debugmalloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "linkedlist.h"

FILE* edges;
FILE* nodes;
mapInfo mapI;

mapInfo GetMapI(void){
    return mapI;
}
void XYconvert(double *x, double *y){
    double relX = mapI.east - mapI.west;
    double relY = mapI.north - mapI.south;

    *x = mapI.imgWidth*((*x - mapI.west)/relX);
    *y = mapI.imgHeight*((mapI.north -*y)/relY);
    
}
void mapItNodes(NodeTomb *mainArray){
    for(int i = 0; i < mainArray->NodeNum; ++i){ 
        XYconvert(&mainArray->nodes[i].x, &mainArray->nodes[i].y);
    }
}
void LineString(Vector2Cell **list0, Vector2Cell **list1, char* str){
    Vector2Double tmp;
    FILE *buffer = fmemopen(str, strlen(str), "r");
    while(fscanf(buffer, "%lf %lf", &tmp.x, &tmp.y)==2){
        XYconvert(&tmp.x, &tmp.y);
        addToListVector(list0, &tmp);
        addToListVector(list1, &tmp);
        if(tmp.x > mapI.imgWidth || tmp.y > mapI.imgHeight){
            printf("%ld\n",strlen(str));
        }
    }
    fclose(buffer);
}
void LineStringSpec(Vector2Cell **list0, char* str){
    Vector2Double tmp;
    FILE *buffer = fmemopen(str, strlen(str), "r");
    while(fscanf(buffer, "%lf %lf", &tmp.x, &tmp.y)==2){
        XYconvert(&tmp.x, &tmp.y);
        addToListVector(list0, &tmp);
        if(tmp.x > mapI.imgWidth || tmp.y > mapI.imgHeight){
            printf("%ld\n",strlen(str));
        }
    }
    fclose(buffer);
}
void NodeInit(Node *a){
    a->hCost=INT_MAX;
    a->gCost=INT_MAX;
    a->parent = NULL;
}
void FreeNodes(NodeTomb *mainArray){
    for (int i = 0; i < mainArray->NodeNum; ++i) {
        for (int j = 0; j < mainArray->nodes[i].fokszam; ++j) {
            DeleteListVector(mainArray->nodes[i].p[j].linestring);
        }
        free(mainArray->nodes[i].p);
    }
    free(mainArray->nodes);
}
void NodeInput(NodeTomb *mainArray){
    nodes = fopen("/home/valbra/Dokumentumok/iskola/prog/Astar/build/datas/nodes1.txt", "r");
    edges = fopen("/home/valbra/Dokumentumok/iskola/prog/Astar/build/datas/edges1.txt", "r");

    if (nodes != NULL) {
        fscanf(nodes, "%d", &mainArray->NodeNum);
        mainArray->nodes = (Node*) malloc(mainArray->NodeNum*sizeof(Node));
        fscanf(nodes, "%lf %lf %lf %lf", &mapI.north, &mapI.south, &mapI.east, &mapI.west);
        mapI.imgHeight = 746;   
        mapI.imgWidth = 1356;

        char tmp[256];
        int degreeSum[mainArray->NodeNum];
        if(edges != NULL){
            int a, b;
            for (int i = 0; i < mainArray->NodeNum; ++i) {
                degreeSum[i] = 0;
            }
            while(fgets(tmp, 256, edges)){
                if(sscanf(tmp, "<edge source=\"%d\" target=\"%d\" id=\"%*d\">", &a, &b)==2){
                    if(a !=b){
                        degreeSum[a]++;
                        degreeSum[b]++;
                    }
                    else{
                        degreeSum[a]++;
                    }
                }
            }
        }
        else{
            perror("Nem sikerült megnyitni az éleket tartalmazó fájlt");
        }
        int id;
        while(fgets(tmp, 256, nodes)) {
            if(sscanf(tmp,"<node id=\"%d\">", &id) ==1){
                mainArray->nodes[id].index= id;
                NodeInit(&mainArray->nodes[id]);
                mainArray->nodes[id].fokszam = degreeSum[id];
                mainArray->nodes[id].p = (Edge*) malloc(degreeSum[id]*sizeof(Edge));
            }
            sscanf(tmp,"<data key=\"d5\">%lf</data>", &mainArray->nodes[id].y);
            sscanf(tmp,"<data key=\"d6\">%lf</data>", &mainArray->nodes[id].x);
        }
        fclose(edges);
    }
    else {
        mainArray->nodes = NULL;
        perror("Nem sikerült megnyitni a pontokat tartalmazó fájlt");
    }
    fclose(nodes);
}
void EdgeInput(NodeTomb *mainArray){
    edges = fopen("/home/valbra/Dokumentumok/iskola/prog/Astar/build/datas/edges1.txt", "r");

    if(edges != NULL){
        char tmp[2000];
        int EdgesIndex[mainArray->NodeNum];
        int a, b;
        double c, d;
        char *str = NULL;
        for (int i = 0; i < mainArray->NodeNum; ++i) {
            EdgesIndex[i] = 0;
        }
        while(fgets(tmp, 2000, edges)){
            sscanf(tmp, "<edge source=\"%d\" target=\"%d\" id=\"%*d\">", &a, &b);
            if(sscanf(tmp, "<data key=\"d12\">%lf", &c) == 1){
                mainArray->nodes[a].p[EdgesIndex[a]].length = c;
                mainArray->nodes[b].p[EdgesIndex[b]].length = c;
            }
            else if(sscanf(tmp, "<data key=\"d13\">%lf", &d)==1){
                mainArray->nodes[a].p[EdgesIndex[a]].time = d;
                mainArray->nodes[b].p[EdgesIndex[b]].time = d;
            }
            else if((str = strchr(tmp, '!'))!= NULL){
                if(a != b){
                    mainArray->nodes[a].p[EdgesIndex[a]].linestring = NULL;
                    mainArray->nodes[b].p[EdgesIndex[b]].linestring = NULL;
                    LineString(&mainArray->nodes[a].p[EdgesIndex[a]].linestring, &mainArray->nodes[b].p[EdgesIndex[b]].linestring, str+1);
                    mainArray->nodes[a].p[EdgesIndex[a]++].p1 = &(mainArray->nodes[b]);
                    mainArray->nodes[b].p[EdgesIndex[b]++].p1 = &(mainArray->nodes[a]);
                }
                else{
                    mainArray->nodes[a].p[EdgesIndex[a]].linestring = NULL;
                    LineStringSpec(&mainArray->nodes[a].p[EdgesIndex[a]].linestring, str+1);
                    mainArray->nodes[a].p[EdgesIndex[a]++].p1 = &(mainArray->nodes[b]);
                }
            }
        }
    }
    else{
        perror("Nem sikerült megnyitni az éleket tartalmazó fájlt");
    }
    fclose(edges);
}