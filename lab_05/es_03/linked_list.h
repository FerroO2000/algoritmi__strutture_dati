#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef void *ll_item_t;
typedef void *ll_item_key_t;
typedef struct _linkedList *linked_list_t;
typedef bool (*ll_match_func)(ll_item_key_t key, ll_item_t currItem);

linked_list_t newLinkedList();
void freeLinkedList(linked_list_t ll);
void pushItem(linked_list_t ll, ll_item_t item);
ll_item_t getItem(linked_list_t ll, ll_item_key_t key, ll_match_func mF);
void printLinkedList(linked_list_t ll, void (*printFunction)(ll_item_t currItem));
ll_item_t removeItem(linked_list_t ll, ll_item_key_t key, ll_match_func mF);

#endif