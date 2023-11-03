#include <stdbool.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "graphElements.h"

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
    temp->next = NULL;
    temp->data = *newData;

    if (list->last != NULL)
        list->last->next = temp;
    else
        list->first = temp;

    temp->previous = list->last;
    list->last = temp;
    return temp;
}
void addToList(List *list, Node *newData) {
    ListCell *temp = (ListCell*)malloc(sizeof(ListCell));
    temp->next = NULL;
    temp->data = *newData;

    if (list->last != NULL)
        list->last->next = temp;
    else
        list->first = temp;

    temp->previous = list->last;
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
    if (toRemove != list->first)
        toRemove->previous->next = toRemove->next;
    else
        list->first = toRemove->next;

    if (toRemove != list->last)
        toRemove->next->previous = toRemove->previous;
    else
        list->last = toRemove->previous;

    free(toRemove);
}
void DeleteList(List *list) {
    while (list->first != NULL)
        removeFromList(list, list->first);
}
