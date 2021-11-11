#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define LINE_BUFFER_SIZE 256
#define STR_BUFFER_SIZE 41
#define CMD_NUM 7

typedef struct
{
    size_t year, month, day;
} date_t;

typedef struct
{
    char *cod, *fistName, *lastName, *street, *city;
    size_t cap;
    date_t birthDate;
} item_t;

typedef struct node
{
    item_t item;
    struct node *next;
} node_t;

typedef struct
{
    node_t *head;
} linked_list_t;

typedef enum
{
    e_add,
    e_add_file,
    e_search,
    e_del_cod,
    e_del_date,
    e_print_file,
    e_exit,
} cmd_e;

void readFile(linked_list_t *l, char *fName);

item_t newItem(char *cod, char *firstName, char *lastName, date_t birthDate, char *street, char *city, size_t cap);
void printItem(item_t *item);
item_t *copyItem(item_t *in);

date_t readDate(char *src);
int compareDate(date_t *a, date_t *b);

void insertNode(linked_list_t *l, item_t item);
item_t *searchNode(linked_list_t *l, char *cod);
item_t *removeNodeByCod(linked_list_t *l, char *cod);
item_t *removeNodeByDate(linked_list_t *l, date_t *start, date_t *end);
node_t *newNode(item_t item, node_t *next);

linked_list_t *newLinkedList();
void printList(linked_list_t *l);

cmd_e readCmd(char cmds[CMD_NUM][STR_BUFFER_SIZE]);
void addCmd(linked_list_t *l);
void addFileCmd(linked_list_t *l);
void searchCmd(linked_list_t *l);
void delCodCmd(linked_list_t *l);
void delDateCmd(linked_list_t *l);
void printFileCmd(linked_list_t *l);

int main()
{
    char cmds[CMD_NUM][STR_BUFFER_SIZE] = {"aggiungi", "aggiungi-file", "cerca", "cancella-cod", "cancella-date", "stampa", "esci"};
    linked_list_t *list = newLinkedList();
    cmd_e cmd;
    bool stop = false;

    do
    {
        cmd = readCmd(cmds);

        switch (cmd)
        {
        case e_add:
            addCmd(list);
            break;
        case e_add_file:
            addFileCmd(list);
            break;
        case e_search:
            searchCmd(list);
            break;
        case e_del_cod:
            delCodCmd(list);
            // printList(list);
            break;
        case e_del_date:
            delDateCmd(list);
            break;
        case e_print_file:
            printFileCmd(list);
            break;

        case e_exit:
            stop = true;
            break;
        default:
            break;
        }
    } while (!stop);

    return 0;
}

void readFile(linked_list_t *l, char *fName)
{
    FILE *fp = fopen(fName, "r");
    if (fp == NULL)
    {
        exit(1);
    }
    char lineBuff[LINE_BUFFER_SIZE], tmpCod[STR_BUFFER_SIZE], tmpFirstName[STR_BUFFER_SIZE], tmpLastName[STR_BUFFER_SIZE], tmpStreet[STR_BUFFER_SIZE], tmpCity[STR_BUFFER_SIZE], tmpBirthDate[STR_BUFFER_SIZE];
    size_t tmpCap;
    while (fgets(lineBuff, LINE_BUFFER_SIZE, fp))
    {
        sscanf(lineBuff, "%s %s %s %s %s %s %d", tmpCod, tmpFirstName, tmpLastName, tmpBirthDate, tmpStreet, tmpCity, &tmpCap);
        insertNode(l, newItem(tmpCod, tmpFirstName, tmpLastName, readDate(tmpBirthDate), tmpStreet, tmpCity, tmpCap));
    }
    fclose(fp);
}

date_t readDate(char *src)
{
    date_t d;
    sscanf(src, "%d/%d/%d", &d.day, &d.month, &d.year);
    return d;
}

int compareDate(date_t *a, date_t *b)
{
    if (a->year > b->year)
    {
        return 1;
    }
    if (a->year < b->year)
    {
        return -1;
    }
    if (a->month > b->month)
    {
        return 1;
    }
    if (a->month < b->month)
    {
        return -1;
    }
    if (a->day > b->day)
    {
        return 1;
    }
    if (a->day < b->day)
    {
        return -1;
    }
    return 0;
}

item_t newItem(char *cod, char *firstName, char *lastName, date_t birthDate, char *street, char *city, size_t cap)
{
    item_t item;
    item.cod = strdup(cod);
    item.fistName = strdup(firstName);
    item.lastName = strdup(lastName);
    item.street = strdup(street);
    item.city = strdup(city);
    item.birthDate = birthDate;
    item.cap = cap;
    return item;
}

void printItem(item_t *item)
{
    printf("\ncod -> %s\nfirst name -> %s\nlast name -> %s\nbirth date -> %02d/%02d/%d\nstreet -> %s\ncity -> %s\ncap -> %d\n",
           item->cod, item->fistName, item->lastName, item->birthDate.day, item->birthDate.month, item->birthDate.year, item->street, item->city, item->cap);
}

void insertNode(linked_list_t *l, item_t item)
{
    if (l->head == NULL)
    {
        l->head = newNode(item, NULL);
        return;
    }

    if (l->head->next == NULL)
    {
        if (compareDate(&item.birthDate, &l->head->item.birthDate) < 0)
        {
            l->head = newNode(item, l->head);
            return;
        }
        l->head->next = newNode(item, NULL);
        return;
    }

    node_t *curr, *prev;
    for (curr = l->head->next, prev = l->head; curr != NULL; prev = curr, curr = curr->next)
    {
        if (compareDate(&item.birthDate, &curr->item.birthDate) < 0)
        {
            prev->next = newNode(item, curr);
            return;
        }
    }
    prev->next = newNode(item, NULL);
}

node_t *newNode(item_t item, node_t *next)
{
    node_t *node = NULL;
    node = (node_t *)malloc(sizeof(node_t));
    node->item = item;
    node->next = next;
    return node;
}

item_t *removeNodeByCod(linked_list_t *l, char *cod)
{
    if (l->head == NULL)
    {
        return NULL;
    }
    if (strcmp(l->head->item.cod, cod) == 0)
    {
        item_t *res = copyItem(&l->head->item);
        l->head = l->head->next;
        return res;
    }
    if (l->head->next == NULL)
    {
        return NULL;
    }
    node_t *curr, *prev;
    for (curr = l->head->next, prev = l->head; curr->next != NULL; prev = curr, curr = curr->next)
    {
        if (strcmp(curr->item.cod, cod) == 0)
        {
            prev->next = curr->next;
            return copyItem(&curr->item);
        }
    }
    return NULL;
}

item_t *removeNodeByDate(linked_list_t *l, date_t *start, date_t *end)
{
    if (l->head == NULL)
    {
        return NULL;
    }
    if (compareDate(&l->head->item.birthDate, start) >= 0 && compareDate(&l->head->item.birthDate, end) <= 0)
    {
        item_t *res = copyItem(&l->head->item);
        l->head = l->head->next;
        return res;
    }
    if (l->head->next == NULL)
    {
        return NULL;
    }
    node_t *curr, *prev;
    for (curr = l->head->next, prev = l->head; curr->next != NULL; prev = curr, curr = curr->next)
    {
        if (compareDate(&curr->item.birthDate, start) >= 0 && compareDate(&curr->item.birthDate, end) <= 0)
        {
            prev->next = curr->next;
            return copyItem(&curr->item);
        }
    }
    return NULL;
}

item_t *copyItem(item_t *in)
{
    item_t *res = (item_t *)malloc(sizeof(*in));
    *res = *in;
    return res;
}

item_t *searchNode(linked_list_t *l, char *cod)
{
    node_t *curr;
    for (curr = l->head; curr->next != NULL; curr = curr->next)
    {
        if (strcmp(curr->item.cod, cod) == 0)
        {
            return copyItem(&curr->item);
        }
    }
    return NULL;
}

linked_list_t *newLinkedList()
{
    linked_list_t *l = (linked_list_t *)malloc(sizeof(linked_list_t));
    l->head = (node_t *)malloc(sizeof(node_t));
    l->head = NULL;
    return l;
}

void printList(linked_list_t *l)
{
    node_t *xp;
    for (xp = l->head; xp != NULL; xp = xp->next)
    {
        printItem(&xp->item);
    }
}

cmd_e readCmd(char cmds[CMD_NUM][STR_BUFFER_SIZE])
{
    size_t i;
    printf("comandi disponibili:");
    for (i = 0; i < CMD_NUM; i++)
    {
        printf(" %s", cmds[i]);
    }
    printf("\n");
    char cmd[STR_BUFFER_SIZE];
    printf("comando: ");
    scanf("%s", cmd);
    for (i = 0; i < CMD_NUM; i++)
    {
        if (strcmp(cmd, cmds[i]) == 0)
        {
            return (cmd_e)i;
        }
    }
    return -1;
}

void addCmd(linked_list_t *l)
{
    char buff[STR_BUFFER_SIZE];
    item_t in;
    printf("codice: ");
    scanf("%s", buff);
    in.cod = strdup(buff);
    printf("nome: ");
    scanf("%s", buff);
    in.fistName = strdup(buff);
    printf("cognome: ");
    scanf("%s", buff);
    in.lastName = strdup(buff);
    printf("data di nascita (gg/mm/aaaa): ");
    scanf("%s", buff);
    in.birthDate = readDate(buff);
    printf("via: ");
    scanf("%s", buff);
    in.street = strdup(buff);
    printf("citta': ");
    scanf("%s", buff);
    in.city = strdup(buff);
    printf("cap': ");
    scanf("%d", &in.cap);
    insertNode(l, in);
}

void addFileCmd(linked_list_t *l)
{
    char fName[STR_BUFFER_SIZE];
    printf("nome del file: ");
    scanf("%s", fName);
    readFile(l, fName);
}

void searchCmd(linked_list_t *l)
{
    char cod[STR_BUFFER_SIZE];
    printf("cod: ");
    scanf("%s", cod);
    item_t *res = searchNode(l, cod);
    if (res == NULL)
    {
        printf("non trovato\n");
        return;
    }
    printItem(res);
}

void delCodCmd(linked_list_t *l)
{
    char cod[STR_BUFFER_SIZE];
    printf("cod: ");
    scanf("%s", cod);
    item_t *res = removeNodeByCod(l, cod);
    if (res == NULL)
    {
        printf("non trovato\n");
    }
    else
    {
        printf("\nrimosso elemento:");
        printItem(res);
    }
}

void delDateCmd(linked_list_t *l)
{
    char startDateStr[STR_BUFFER_SIZE], endDateStr[STR_BUFFER_SIZE];
    printf("data iniziale: ");
    scanf("%s", startDateStr);
    printf("data finale: ");
    scanf("%s", endDateStr);
    date_t startDate = readDate(startDateStr), endDate = readDate(endDateStr);
    item_t *res = removeNodeByDate(l, &startDate, &endDate);
    bool foundOne = false;
    while (res != NULL)
    {
        printf("\nrimosso elemento:");
        printItem(res);
        foundOne = true;
        res = removeNodeByDate(l, &startDate, &endDate);
        if (res == NULL)
        {
            break;
        }
    }
    if (!foundOne)
    {
        printf("non trovato\n");
    }
}

void printFileCmd(linked_list_t *l)
{
    FILE *fp;
    fp = fopen("risultati.txt", "w");
    if (fp == NULL)
    {
        exit(1);
    }

    node_t *curr;
    for (curr = l->head; curr != NULL; curr = curr->next)
    {
        item_t tmp = curr->item;
        fprintf(fp, "%s %s %s %02d/%02d/%d %s %s %d\n", tmp.cod, tmp.fistName, tmp.lastName, tmp.birthDate.day, tmp.birthDate.month, tmp.birthDate.year, tmp.street, tmp.city, tmp.cap);
    }

    fclose(fp);
}