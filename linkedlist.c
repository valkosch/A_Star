#include <stdbool.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "graphElements.h"
#include "debugmalloc.h"

bool Contains(List *list, Node *data){
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

ListCell *addToListSpec(List *list, Node *newData, ListCell *temp) {
    temp = malloc(sizeof(ListCell));
    temp->data = *newData;
    temp->next = NULL;
    temp->previous = list->last;

    if (list->last == NULL){
        list->first = temp;
    }
    else{
        list->last->next = temp;
    }

    list->last = temp;
    return temp;
}
void addToList(List *list, Node *newData) {
    ListCell *temp = (ListCell*)malloc(sizeof(ListCell));
    temp->data = *newData;
    temp->next = NULL;
    temp->previous = list->last;

    if (list->last == NULL){
        list->first = temp;
    }
    else{
        list->last->next = temp;
    }
    list->last = temp;
}
int Count(List *list){
    int x = 0;
    ListCell y;
    y.next = list ->first;
    while(y.next != NULL){
        x++;
        y.next = y.next->next;
    }
    return x;
}
void removeFromList(List *list, ListCell *toRemove) {
    if (toRemove == list->last){
        list->last = toRemove->previous;
    }
    else{
        toRemove->next->previous = toRemove->previous;
    }
    if (toRemove == list->first){
        list->first = toRemove->next;
    }
    else{
        toRemove->previous->next = toRemove->next;
    }
    free(toRemove);
}
void DeleteList(List *list) {
    while (list->last != NULL)
        removeFromList(list, list->first);
}
