#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    char hColor, vColor;
    int hValue, vValue;
    bool inUse;
} tile_t;

typedef struct
{
    int rows, cols;
    tile_t *tiles;
} board_t;

tile_t rotateTile(tile_t *t);
board_t *makeBoard(int r, int c);
void freeBoard(board_t *b);
void insertTile(board_t *b, tile_t t, int row, int col);
void printBoard(board_t *b);
tile_t *getBoardTile(board_t *b, int row, int col);
board_t *copyBoard(board_t *in);
int getScore(board_t *b);
board_t *solutionRec(tile_t *t, bool *tileMask, int currTile, int tileNum, int currBoardPos, board_t *currBoard);

int main()
{
    FILE *fp = fopen("tiles.txt", "r");
    if (fp == NULL)
    {
        return 1;
    }
    int tileNum, i;
    tile_t *tiles;
    fscanf(fp, "%d\n", &tileNum);
    tiles = (tile_t *)calloc(tileNum, sizeof(tile_t));
    for (int i = 0; i < tileNum; i++)
    {
        fscanf(fp, "%c %d %c %d\n", &tiles[i].hColor, &tiles[i].hValue, &tiles[i].vColor, &tiles[i].vValue);
        tiles[i].inUse = false;
    }
    fclose(fp);

    fp = fopen("board.txt", "r");
    if (fp == NULL)
    {
        return 1;
    }
    int r, c, j, tileIdx, direction, tmpVal, usedTileCount = 0;
    char tmpClr;
    fscanf(fp, "%d %d\n", &r, &c);
    board_t *board = makeBoard(r, c);
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            fscanf(fp, "%d/%d", &tileIdx, &direction);
            if (tileIdx >= 0)
            {
                usedTileCount++;
                tiles[tileIdx].inUse = true;
                if (direction == 1)
                {
                    insertTile(board, rotateTile(&tiles[tileIdx]), i, j);
                    continue;
                }
                insertTile(board, tiles[tileIdx], i, j);
                continue;
            }
        }
    }

    printBoard(board);
    printf("\n\n");

    fclose(fp);

    int unTiles = (tileNum - usedTileCount);
    tile_t unusedTiles[unTiles * 2];
    bool mask[unTiles * 2];
    j = 0;
    for (i = 0; i < tileNum; i++)
    {
        if (tiles[i].inUse)
        {
            continue;
        }
        unusedTiles[j] = tiles[i];
        mask[j] = false;
        j++;
    }
    for (i = 0; i < unTiles; i++)
    {
        unusedTiles[i + unTiles] = rotateTile(&unusedTiles[i]);
        mask[i + unTiles] = false;
    }

    board_t *fBoard = solutionRec(unusedTiles, mask, 0, unTiles * 2, 0, board);

    printBoard(fBoard);

    printf("%d", getScore(fBoard));

    freeBoard(board);

    return 0;
}

tile_t rotateTile(tile_t *t)
{
    tile_t res;
    res.inUse = t->inUse;
    res.hColor = t->vColor;
    res.vColor = t->hColor;
    res.hValue = t->vValue;
    res.vValue = t->hValue;
    return res;
}

board_t *makeBoard(int r, int c)
{
    board_t *b = (board_t *)malloc(sizeof(board_t));
    b->rows = r;
    b->cols = c;
    b->tiles = (tile_t *)calloc(r * c, sizeof(tile_t));
    return b;
}

void freeBoard(board_t *b)
{
    free(b->tiles);
    free(b);
}

void insertTile(board_t *b, tile_t t, int row, int col)
{
    b->tiles[row * (b->rows) + col] = t;
}

tile_t *getBoardTile(board_t *b, int row, int col)
{
    return &b->tiles[row * (b->rows) + col];
}

void printBoard(board_t *b)
{
    int i, j;
    for (i = 0; i < b->rows; i++)
    {
        printf("\n");
        for (j = 0; j < b->cols; j++)
        {
            if (!getBoardTile(b, i, j)->inUse)
            {
                printf("nnnnnnnnnn   ");
                continue;
            }
            printf("%c %d --- %c %d   ", getBoardTile(b, i, j)->hColor, getBoardTile(b, i, j)->hValue, getBoardTile(b, i, j)->vColor, getBoardTile(b, i, j)->vValue);
        }
    }
}

int getScore(board_t *b)
{
    int i, j, tmp, score = 0;
    char prevClr;
    for (i = 0; i < b->rows; i++)
    {
        tmp = 0;
        for (j = 0; j < b->cols; j++)
        {
            if (j == 0)
            {
                prevClr = getBoardTile(b, i, j)->hColor; //b->tiles[i + j].hColor;
                tmp += getBoardTile(b, i, j)->hValue;
                continue;
            }
            if (prevClr != getBoardTile(b, i, j)->hColor)
            {
                tmp = 0;
                break;
            }
            tmp += getBoardTile(b, i, j)->hValue;
        }
        score += tmp;
    }

    for (i = 0; i < b->cols; i++)
    {
        tmp = 0;
        for (j = 0; j < b->rows; j++)
        {
            if (j == 0)
            {
                prevClr = getBoardTile(b, j, i)->vColor;
                tmp += getBoardTile(b, j, i)->vValue;
                continue;
            }
            if (prevClr != getBoardTile(b, j, i)->vColor)
            {
                tmp = 0;
                break;
            }
            tmp += getBoardTile(b, j, i)->vValue;
        }
        score += tmp;
    }

    return score;
}

board_t *copyBoard(board_t *in)
{
    board_t *res = malloc(sizeof(board_t));
    res->cols = in->cols;
    res->rows = in->rows;
    int nTiles = in->rows * (in->cols);
    res->tiles = calloc(nTiles, sizeof(tile_t));
    int i;
    for (i = 0; i < nTiles; i++)
    {
        res->tiles[i] = in->tiles[i];
    }
    return res;
}

board_t *solutionRec(tile_t *t, bool *tileMask, int currTile, int tileNum, int currBoardPos, board_t *currBoard)
{
    if (currTile >= tileNum)
    {

        /*printf("\n");
        printBoard(currBoard);
        printf("\nscore: %d", getScore(currBoard));*/
        return currBoard;
    }

    if (currBoard->tiles[currBoardPos].inUse)
    {
        return solutionRec(t, tileMask, currTile, tileNum, currBoardPos + 1, currBoard);
    }

    int i, bestScore = -1, tmpScore;
    board_t *tmpB = makeBoard(currBoard->rows, currBoard->cols), *bestB = makeBoard(currBoard->rows, currBoard->cols);

    for (i = 0; i < tileNum; i++)
    {
        if (tileMask[i])
        {
            continue;
        }
        tileMask[i] = true;
        t[i].inUse = true;
        currBoard->tiles[currBoardPos] = t[i];
        currBoard->tiles[currBoardPos].inUse = true;
        //printf("\n");
        //printBoard(currBoard);
        tmpB = solutionRec(t, tileMask, currTile + 1, tileNum, currBoardPos + 1, currBoard);
        tmpScore = getScore(tmpB);
        if (tmpScore > bestScore)
        {
            bestScore = tmpScore;
            bestB = copyBoard(tmpB); //tmpB;
        }
        tileMask[i] = false;
        currBoard->tiles[currBoardPos].inUse = false;
    }

    return bestB;

    /*board_t *tmpB = malloc(sizeof(currBoard)), *bestB = malloc(sizeof(currBoard));
    for (i = currTile; i < tileNum; i++)
    {
        currBoard[currBoardPos] = t[i];
        tmpB = solutionRec(t, currTile + 1, tileNum, bRow, bCol, currBoard + 1, currBoard);
        tmpScore = getScore(tmpB, bRow, bCol);
        if (tmpScore > bestScore)
        {
            bestScore = tmpScore;
            bestB = tmpB;
        }
    }

    return bestB;*/
}