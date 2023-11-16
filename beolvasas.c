
#include "beolvasas.h"
#include "debugmalloc.h"
#include <stdlib.h>
#include <limits.h>
#include "graphElements.h"

FILE* edges;
FILE* nodes;
mapInfo mapI;

mapInfo GetMapI(void){
    return mapI;
}
void mapItNodes(Node *tomb, int const *l){
    double relX = mapI.east - mapI.west;
    double relY = mapI.north - mapI.south;
    for(int i = 0; i < *l; ++i){
        tomb[i].x = mapI.imgWidth*((tomb[i].x - mapI.west)/relX);
        tomb[i].y= mapI.imgHeight*((mapI.north -tomb[i].y)/relY);

    }
}
void NodeInit(Node *a){
    a->hCost=INT_MAX;
    a->gCost=INT_MAX;
    a->parent = NULL;
}
void FreeNodes(int l, Node *t){
    for (int i = 0; i < l ; ++i) {
        free(t[i].p);
    }
    free(t);
}
Node *NodeInput(int *nodeNum){
    nodes = fopen("datas/nodes1.txt", "r");
    edges = fopen("datas/edges1.txt", "r");
    Node *tomb;

    if (nodes != NULL) {
        fscanf(nodes, "%d", nodeNum);
        tomb = (Node*) malloc(*nodeNum*sizeof(Node));
        fscanf(nodes, "%lf %lf %lf %lf", &mapI.north, &mapI.south, &mapI.east, &mapI.west);
        mapI.imgHeight = 746;   
        mapI.imgWidth = 1356;

        char tmp[256];
        int degreeSum[*nodeNum];
        if(edges != NULL){
            int a, b;
            for (int i = 0; i < *nodeNum; ++i) {
                degreeSum[i] = 0;
            }
            while(fgets(tmp, 256, edges)){
                if(sscanf(tmp, "<edge source=\"%d\" target=\"%d\" id=\"%*d\">", &a, &b)==2){
                    degreeSum[a]++;
                    degreeSum[b]++;
                }
            }
        }
        else{
            perror("Nem sikerült megnyitni az éleket tartalmazó fájlt");
        }
        int id;
        while(fgets(tmp, 256, nodes)) {
            if(sscanf(tmp,"<node id=\"%d\">", &id) ==1){
                tomb[id].index= id;
                NodeInit(&tomb[id]);
                tomb[id].fokszam = degreeSum[id];
                tomb[id].p = (Edge*) malloc(degreeSum[id]*sizeof(Edge));
            }
            sscanf(tmp,"<data key=\"d5\">%lf</data>", &tomb[id].y);
            sscanf(tmp,"<data key=\"d6\">%lf</data>", &tomb[id].x);
        }
        fclose(edges);
    }
    else {
        tomb = NULL;
        perror("Nem sikerült megnyitni a pontokat tartalmazó fájlt");
    }
    fclose(nodes);
    return tomb;
}
void EdgeInput(Node *tomb, int const *m){
    edges = fopen("datas/edges1.txt", "r");

    if(edges != NULL){
        char tmp[256];
        int EdgesIndex[*m];
        char *str;
        int a, b;
        double c, d;
        for (int i = 0; i < *m; ++i) {
            EdgesIndex[i] = 0;
        }
        while(fgets(tmp, 256, edges)){
            sscanf(tmp, "<edge source=\"%d\" target=\"%d\" id=\"%*d\">", &a, &b);
            if(sscanf(tmp, "<data key=\"d12\">%lf</data>", &c) == 1){
                tomb[a].p[EdgesIndex[a]].length = c;
                tomb[b].p[EdgesIndex[b]].length = c;
            }
            if(sscanf(tmp, "<data key=\"d13\">%lf</data>", &d)==1){
                tomb[a].p[EdgesIndex[a]].time = d;
                tomb[b].p[EdgesIndex[b]].time = d;
                tomb[a].p[EdgesIndex[a]++].p1 = &tomb[b];
                tomb[b].p[EdgesIndex[b]++].p1 = &tomb[a];
            }
        }
    }
    else{
        perror("Nem sikerült megnyitni az éleket tartalmazó fájlt");
    }
    fclose(edges);
}