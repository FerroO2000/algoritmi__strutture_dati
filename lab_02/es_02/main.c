#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILE_NAME "mat.txt"

int **malloc2dP(int *nRows, int *nCols);
void free2d(int **m, int nr);
void separa(int **mat, int nr, int nc, int **white, int **black);
int getWhiteCount(int nr, int nc);
int getBlackCount(int nr, int nc);

int main()
{
    int nr, nc;
    int **mat = malloc2dP(&nr, &nc);
    int *white, *black;
    separa(mat, nr, nc, &white, &black);

    int i;
    printf("bianchi: ");
    for (i = 0; i < getWhiteCount(nr, nc); i++)
    {
        printf("%d ", white[i]);
    }
    printf("\nneri: ");
    for (i = 0; i < getBlackCount(nr, nc); i++)
    {
        printf("%d ", black[i]);
    }

    free(white);
    free(black);
    free2d(mat, nr);

    return 0;
}

int **malloc2dP(int *nRows, int *nCols)
{
    FILE *fIn = fopen(FILE_NAME, "r");
    if (fIn == NULL)
    {
        exit(1);
    }

    int nr, nc, **mat, i, j;
    fscanf(fIn, "%d %d\n", &nr, &nc);
    mat = (int **)calloc(nr, sizeof(int *));

    for (i = 0; i < nr; i++)
    {
        mat[i] = (int *)calloc(nc, sizeof(int));
        for (j = 0; j < nc; j++)
        {
            fscanf(fIn, "%d", &mat[i][j]);
        }
    }

    fclose(fIn);

    *nRows = nr;
    *nCols = nc;
    return mat;
}

void free2d(int **m, int nr)
{
    int i;
    for (i = 0; i < nr; i++)
    {
        free(m[i]);
    }
    free(m);
}

int getWhiteCount(int nr, int nc)
{
    if (nc % 2 == 0)
    {
        return nr * (nc / 2);
    }
    if (nr % 2 == 0)
    {
        return nr * ((nc + 1) / 2) - nr / 2;
    }
    return nr * ((nc + 1) / 2) - (nr - 1) / 2;
}

int getBlackCount(int nr, int nc)
{
    if (nc % 2 == 0)
    {
        return getWhiteCount(nr, nc);
    }
    if (nr % 2 == 0)
    {
        return getWhiteCount(nr, nc);
    }
    return getWhiteCount(nr, nc) - 1;
}

void separa(int **mat, int nr, int nc, int **white, int **black)
{
    int nWhite = getWhiteCount(nr, nc), nBlack = getBlackCount(nr, nc);
    int i, j, wCount = 0, bCount = 0;
    int *tmpWhite, *tmpBlack;

    tmpWhite = (int *)calloc(nWhite, sizeof(int));
    tmpBlack = (int *)calloc(nBlack, sizeof(int));

    for (i = 0; i < nr; i++)
    {
        for (j = 0; j < nc; j++)
        {
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    tmpWhite[wCount] = mat[i][j];
                    wCount++;
                }
                else
                {
                    tmpBlack[bCount] = mat[i][j];
                    bCount++;
                }
            }
            else
            {
                if (j % 2 != 0)
                {
                    tmpWhite[wCount] = mat[i][j];
                    wCount++;
                }
                else
                {
                    tmpBlack[bCount] = mat[i][j];
                    bCount++;
                }
            }
        }
    }

    *white = tmpWhite;
    *black = tmpBlack;
}