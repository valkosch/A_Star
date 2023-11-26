#include <stdbool.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "graphElements.h"
#include "debugmalloc.h"

bool Contains(List *list, Node *data){ /*tartalmazza-e? fgv. a list struktúrán */
    ListCell y;
    y.next = list ->first;
    while(y.next != NULL){
        if(y.next->data.x == data->x && y.next->data.y == data->y){
            return true;
        }
        y.next = y.next->next;
    }
    return false;
}

 void addToListSpec(List *list, Node *newData, ListCell **temp) { /*az addToList-nek egy speciális változata ahol temp étéke az az új memóriaterületre mutató pointer lesz*/
    *temp = (ListCell*)malloc(sizeof(ListCell));
    (*temp)->data = *newData;
    (*temp)->next = NULL;
    (*temp)->previous = list->last;

    if (list->last == NULL){
        list->first = *temp;
    }
    else{
        list->last->next = *temp;
    }

    list->last = *temp;
}
void addToList(List *list, Node *newData) { /*lista végéhez fűzést elvégző fgv.*/
    ListCell *temp = (ListCell*)malloc(sizeof(ListCell));
    temp->data = *newData;
    temp->next = NULL;
    temp->previous = list->last;

    if (list->last == NULL){ /*ha a lista üres, akkor az új elem legyen az első elem (és az utolsó is egyszerre)*/
        list->first = temp;
    }
    else{
        list->last->next = temp; /*máskülönben az utolső elem mutasson az újra*/
    }
    list->last = temp;
}
void addToListVector(Vector2Cell **list, Vector2Double *newData) { /*a vektorok listájához előre befűzés*/
    Vector2Cell *temp = (Vector2Cell*)malloc(sizeof(Vector2Cell));
    temp->data = *newData;
    temp->next = *list;
    *list = temp;
}
int Count(List *list){ /*lista elemeinek db számát megadja*/
    int x = 0;
    ListCell y;
    y.next = list ->first;
    while(y.next != NULL){
        x++;
        y.next = y.next->next;
    }
    return x;
}
void removeFromList(List *list, ListCell *toRemove) { /*lista tetszőleges elemének törlése*/
    if (toRemove == list->last){ /*ha az utolsó a törlendő akkor az utolsó előtti legyen az új utolsó*/
        list->last = toRemove->previous;
    }
    else{
        toRemove->next->previous = toRemove->previous; /*máskülönben törlendő utáni mutasson az a törlendő előttire*/
    }
    if (toRemove == list->first){ /*ha a törlendő az első akkor a törlendő utáni legyen az új első*/
        list->first = toRemove->next;
    }
    else{
        toRemove->previous->next = toRemove->next; /*máskülönben a törlendő előtti mutasson a törlendő utánira*/
    }
    free(toRemove);
}
void DeleteList(List *list) { /*egész lista felszabadítása*/
    while (list->last != NULL)
        removeFromList(list, list->first);
}
void DeleteListVector(Vector2Cell *list) {
    while(list != NULL){
        Vector2Cell *mozgo = list->next;
        free(list);
        list = mozgo;
    }
}
