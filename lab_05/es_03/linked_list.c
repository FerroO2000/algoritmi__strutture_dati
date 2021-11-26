#include "linked_list.h"

typedef struct node
{
    ll_item_t item;
    struct node *next;
} _node_t;

struct _linkedList
{
    _node_t *head;
    _node_t *tail;
};

_node_t *_newNode(ll_item_t item, _node_t *next);

linked_list_t newLinkedList()
{
    linked_list_t newLL = malloc(sizeof(struct _linkedList));
    newLL->head = (_node_t *)malloc(sizeof(_node_t));
    newLL->tail = (_node_t *)malloc(sizeof(_node_t));
    newLL->head = NULL;
    newLL->tail = NULL;
    return newLL;
}

void freeLinkedList(linked_list_t ll)
{
    free(ll->head);
    free(ll->tail);
    free(ll);
}

void pushItem(linked_list_t ll, ll_item_t item)
{
    _node_t *newNode = _newNode(item, NULL);
    if (ll->head == NULL)
    {
        ll->head = newNode;
        ll->tail = newNode;
        return;
    }
    ll->tail->next = newNode;
    ll->tail = newNode;
}

_node_t *_newNode(ll_item_t item, _node_t *next)
{
    _node_t *newNode = malloc(sizeof(_node_t));
    newNode->item = item;
    newNode->next = next;
    return newNode;
}

ll_item_t getItem(linked_list_t ll, ll_item_key_t key, bool (*matchFunction)(ll_item_key_t key, ll_item_t currItem))
{
    _node_t *currNode = NULL;
    for (currNode = ll->head; currNode != NULL; currNode = currNode->next)
    {
        if (matchFunction(key, currNode->item))
        {
            return currNode->item;
        }
    }
    return NULL;
}

void printLinkedList(linked_list_t ll, void (*printFunction)(ll_item_t currItem))
{
    _node_t *currNode = NULL;
    for (currNode = ll->head; currNode != NULL; currNode = currNode->next)
    {
        printFunction(currNode->item);
        printf("\n");
    }
}

ll_item_t removeItem(linked_list_t ll, ll_item_key_t key, ll_match_func mF)
{
    _node_t *currNode = NULL, *prevNode = NULL;
    for (currNode = ll->head; currNode != NULL; prevNode = currNode, currNode = currNode->next)
    {
        if (mF(key, currNode->item))
        {
            if (prevNode == NULL)
            {
                ll->head = currNode->next;
            }
            else
            {
                prevNode->next = currNode->next;
            }
            return currNode->item;
        }
    }
    return NULL;
}