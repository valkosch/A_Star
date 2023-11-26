
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

mapInfo GetMapI(void){ /*függvény ami lehetővé teszi hogy bármelyik modulban elérjem a mapI-t*/
    return mapI;
}
void XYconvert(double *x, double *y){ /*szélességi és hosszúsági fok arányos átváltása a képernyő koordináta rendszerének megfelelő pontjaira */
    double relX = mapI.east - mapI.west;
    double relY = mapI.north - mapI.south;

    *x = mapI.imgWidth*((*x - mapI.west)/relX);
    *y = mapI.imgHeight*((mapI.north -*y)/relY);
    
}
void mapItNodes(NodeTomb *mainArray){ /*az összes pont (útkereszteződés) koordinátáinak átváltása*/
    for(int i = 0; i < mainArray->NodeNum; ++i){ 
        XYconvert(&mainArray->nodes[i].x, &mainArray->nodes[i].y);
    }
}
void LineString(Vector2Cell **list0, Vector2Cell **list1, char* str){/*az élek pontos vonalkövetésének eltárolása (görbeutaknál szükséges mert ponttól pontig húzott vonalak nem jók)*/
    Vector2Double tmp;
    int i = 0;
    double a;
    char *end;
    while((a = strtod(str, &end))!=0){ 
        /*a str-ből tetszőleges mennyiségű koordináta olvasható be ezzel a módszerrel
        mivel beolvas egy double-t aztán a következőre ugrik, ezt addig amig van mit olvasni, a párokból az 1. lesz az x, 2. pedig y*/
        if(i % 2 == 0){
            tmp.x = a;
        }
        else{
            tmp.y = a;
            XYconvert(&tmp.x, &tmp.y);
            addToListVector(list0, &tmp);
            addToListVector(list1, &tmp);
        }
        i++;
        str = end;
    }
}
void LineStringSpec(Vector2Cell **list0, char* str){ /*ugyanaz mint az előző függvény csak arra a speciális esetre mikor hurokél van és így csak egy listáról van szó*/
    Vector2Double tmp;
    int i = 0;
    double a;
    char *end;
    while((a = strtod(str, &end))!=0){
        if(i % 2 == 0){
            tmp.x = a;
        }
        else{
            tmp.y = a;
            XYconvert(&tmp.x, &tmp.y);
            addToListVector(list0, &tmp);
        }
        i++;
        str = end;
    }
}
void NodeInit(Node *a){ /*visszaállítja a pontok mezőinek a kezdőértékét amik esetle változhattak az A* lefutása közben*/
    a->hCost=INT_MAX;
    a->gCost=INT_MAX;
    a->parent = NULL;
}
void FreeNodes(NodeTomb *mainArray){ /*felszabadítja a főtömböt, azaz a pontokat és a pontokhoz tartozó élek tömbjét*/
    for (int i = 0; i < mainArray->NodeNum; ++i) {
        for (int j = 0; j < mainArray->nodes[i].fokszam; ++j) {
            DeleteListVector(mainArray->nodes[i].p[j].linestring);
        }
        free(mainArray->nodes[i].p);
    }
    free(mainArray->nodes);
}
void NodeInput(NodeTomb *mainArray){ /*lefoglalja és beolvassa a főtömbbe a pontokat a "nodes1.txt"-ből*/
    nodes = fopen("nodes1.txt", "r");
    edges = fopen("edges1.txt", "r"); /*megnyitja az "egdes1.txt"-t is mert minden pontnál előre lefoglalja az éleknek is a helyet, ehhez viszont meg kell számoli mennyi él van pontonként*/

    if (nodes != NULL) {
        fscanf(nodes, "%d", &mainArray->NodeNum); /*beolvassa a pontok számát a txt-ből*/
        mainArray->nodes = (Node*) malloc(mainArray->NodeNum*sizeof(Node)); /*lefoglalja*/
        fscanf(nodes, "%lf %lf %lf %lf", &mapI.north, &mapI.south, &mapI.east, &mapI.west); /*térkép tulajdonságainak beolvasása*/
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
                if(sscanf(tmp, "<edge source=\"%d\" target=\"%d\" id=\"%*d\">", &a, &b)==2){ /*megszámolja az éleket pontonként, kezeli a hurokéleket*/
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
            if(sscanf(tmp,"<node id=\"%d\">", &id) ==1){ /*minden node(pont)-nak lefoglalja az éleinek a memóriát illetve beolvassa x és y koordinátáit*/
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
void EdgeInput(NodeTomb *mainArray){ /*élek beolvasása*/
    edges = fopen("edges1.txt", "r");

    if(edges != NULL){
        char tmp[2000];
        int EdgesIndex[mainArray->NodeNum]; /*az EdgeIndex[id] számolja ogy az id-edik pont hanyadik élénel tart a beolvasásnál */
        int a, b;
        double c, d;
        char *str = NULL;
        for (int i = 0; i < mainArray->NodeNum; ++i) {
            EdgesIndex[i] = 0;
        }
        while(fgets(tmp, 2000, edges)){
            sscanf(tmp, "<edge source=\"%d\" target=\"%d\" id=\"%*d\">", &a, &b); /*egyzserre olvassuk be az a->b él és b->a él tulajdonságait mivel megegyezőek*/
            if(sscanf(tmp, "<data key=\"d12\">%lf", &c) == 1){
                mainArray->nodes[a].p[EdgesIndex[a]].length = c; /*length mint úthossz*/
                mainArray->nodes[b].p[EdgesIndex[b]].length = c;
            }
            else if(sscanf(tmp, "<data key=\"d13\">%lf", &d)==1){
                mainArray->nodes[a].p[EdgesIndex[a]].time = d; /*time mint idő amennyi idő alatt meg lehet tenni az utat*/
                mainArray->nodes[b].p[EdgesIndex[b]].time = d;
            }
            else if((str = strchr(tmp, '!'))!= NULL){
                if(a != b){
                    mainArray->nodes[a].p[EdgesIndex[a]].linestring = NULL; /*linestring koordináták sorához érkezve '!' től mát csak a koordináták vannak és az strchr() azt a pointert adja vissza ami pont ezeknek a koordináták stringjére mutat*/
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