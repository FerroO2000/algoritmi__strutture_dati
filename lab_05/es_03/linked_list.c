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
    ll_free_func freeFunc;
    ll_print_func printFunc;
    ll_match_func matchFunc;
};

_node_t *_newNode(ll_item_t item, _node_t *next);
void _freeNode(_node_t *node, ll_free_func freeFunc);

linked_list_t newLinkedList(ll_free_func freeFunc, ll_match_func matchFunc, ll_print_func printFunc)
{
    linked_list_t newLL = malloc(sizeof(struct _linkedList));
    newLL->head = NULL;
    newLL->tail = NULL;
    newLL->freeFunc = freeFunc;
    newLL->printFunc = printFunc;
    newLL->matchFunc = matchFunc;
    return newLL;
}

void freeLinkedList(linked_list_t ll)
{
    _node_t *currNode = NULL, *nextNode = NULL;
    for (currNode = ll->head; currNode != NULL; currNode = nextNode)
    {
        nextNode = currNode->next;
        _freeNode(currNode, ll->freeFunc);
    }
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

void _freeNode(_node_t *node, ll_free_func freeFunc)
{
    freeFunc(node->item);
    free(node);
}

ll_item_t getItem(linked_list_t ll, ll_item_key_t key)
{
    _node_t *currNode = NULL;
    for (currNode = ll->head; currNode != NULL; currNode = currNode->next)
    {
        if (ll->matchFunc(key, currNode->item))
        {
            return currNode->item;
        }
    }
    return NULL;
}

void printLinkedList(linked_list_t ll)
{
    _node_t *currNode = NULL;
    for (currNode = ll->head; currNode != NULL; currNode = currNode->next)
    {
        ll->printFunc(currNode->item);
        printf("\n");
    }
}

ll_item_t removeItem(linked_list_t ll, ll_item_key_t key)
{
    _node_t *currNode = NULL, *prevNode = NULL;
    for (currNode = ll->head; currNode != NULL; prevNode = currNode, currNode = currNode->next)
    {
        if (ll->matchFunc(key, currNode->item))
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