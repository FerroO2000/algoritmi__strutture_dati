#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int vertexes[2];
} edge_t;

typedef struct
{
    int number, edgeCount;
    edge_t *edges;
} graph_t;

bool isVertexCover(graph_t *gList, int n, int e);
void printGraphs(graph_t *gList, int n);
void printVertexCovers(graph_t *gList, int n, int e);

int main()
{
    FILE *fp = fopen("grafo.txt", "r");
    if (fp == NULL)
    {
        return 1;
    }

    int n, e, i;
    fscanf(fp, "%d %d", &n, &e);
    graph_t gList[n];
    for (i = 0; i < n; i++)
    {
        gList[i].number = i;
        gList[i].edgeCount = 0;
        gList[i].edges = (edge_t *)calloc(e, sizeof(edge_t));
    }

    int v1, v2;
    for (i = 0; i < e; i++)
    {
        fscanf(fp, "%d %d", &v1, &v2);
        gList[v1].edges[gList[v1].edgeCount].vertexes[0] = v1;
        gList[v1].edges[gList[v1].edgeCount].vertexes[1] = v2;
        gList[v1].edgeCount++;
        gList[v2].edges[gList[v2].edgeCount].vertexes[0] = v2;
        gList[v2].edges[gList[v2].edgeCount].vertexes[1] = v1;
        gList[v2].edgeCount++;
    }

    printf("vertex cover CON DUPLICATI:\n");
    printVertexCovers(gList, n, e);

    for (i = 0; i < e; i++)
    {
        free(gList[i].edges);
    }

    return 0;
}

bool isVertexCover(graph_t *gList, int n, int e)
{
    bool visited[e], canSet;
    int i, j, k, vCount = 0, eCount, eMask[e][2], tmp, v1, v2;
    for (i = 0; i < e; i++)
    {
        visited[i] = false;
    }
    for (i = 0; i < n; i++)
    {
        eCount = gList[i].edgeCount;
        for (j = 0; j < eCount; j++)
        {
            v1 = gList[i].edges[j].vertexes[0];
            v2 = gList[i].edges[j].vertexes[1];
            if (v1 > v2)
            {
                tmp = v1;
                v1 = v2;
                v2 = tmp;
            }
            canSet = true;
            for (k = 0; k < vCount; k++)
            {
                if (eMask[k][0] == v1 && eMask[k][1] == v2)
                {
                    canSet = false;
                    break;
                }
            }
            if (canSet)
            {
                eMask[vCount][0] = v1;
                eMask[vCount][1] = v2;
                visited[vCount] = true;
                vCount++;
            }
        }
    }
    for (i = 0; i < e; i++)
    {
        if (!visited[i])
        {
            return false;
        }
    }
    return true;
}

void printGraphs(graph_t *gList, int n)
{
    int i;
    printf("(");
    for (i = 0; i < n; i++)
    {
        printf(" %d ", gList[i].number);
    }
    printf(")\n");
}

void printVertexCovers(graph_t *gList, int n, int e)
{

    if (isVertexCover(gList, n, e))
    {
        printGraphs(gList, n);
    }

    if (n == 1)
    {
        return;
    }

    int i, j, z;
    graph_t tmpGList[n - 1];
    for (i = 0; i < n; i++)
    {
        z = 0;
        for (j = 0; j < n; j++)
        {
            if (j == i)
            {
                continue;
            }
            tmpGList[z] = gList[j];
            z++;
        }
        printVertexCovers(tmpGList, n - 1, e);
    }
}