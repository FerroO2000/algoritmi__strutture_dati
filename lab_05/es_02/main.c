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

void printSolution(tile_t *tiles, int tileNum, int usedTileNum, board_t *initialBoard);
tile_t rotateTile(tile_t *t);
board_t *makeBoard(int r, int c);
void freeBoard(board_t *b);
void insertTile(board_t *b, tile_t t, int row, int col);
void printBoard(board_t *b);
tile_t *getBoardTile(board_t *b, int row, int col);
board_t *copyBoard(board_t *in);
int getScore(board_t *b);
board_t *solutionRec(tile_t *t, int currTile, int tileNum, int currBoardPos, board_t *currBoard);

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

    fclose(fp);

    printSolution(tiles, tileNum, usedTileCount, board);

    freeBoard(board);
    free(tiles);

    return 0;
}

void printSolution(tile_t *tiles, int tileNum, int usedTileNum, board_t *initialBoard)
{
    int i, j = 0, unusedTileNum = (tileNum - usedTileNum);
    tile_t unusedTiles[unusedTileNum];
    for (i = 0; i < tileNum; i++)
    {
        if (tiles[i].inUse)
        {
            continue;
        }
        unusedTiles[j] = tiles[i];
        j++;
    }

    printf("scacchiera iniziale:\n");
    printBoard(initialBoard);

    board_t *finalBoard = solutionRec(unusedTiles, 0, unusedTileNum, 0, initialBoard);
    printf("\n\n\n\nsoluzione:\n");
    printBoard(finalBoard);
    printf("\n\npunteggio: %d", getScore(finalBoard));

    freeBoard(finalBoard);
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
                printf("   ---------   ");
                continue;
            }
            printf("   %c %d / %c %d   ", getBoardTile(b, i, j)->hColor, getBoardTile(b, i, j)->hValue, getBoardTile(b, i, j)->vColor, getBoardTile(b, i, j)->vValue);
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
                prevClr = getBoardTile(b, i, j)->hColor;
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

board_t *solutionRec(tile_t *t, int currTile, int tileNum, int currBoardPos, board_t *currBoard)
{
    if (currTile >= tileNum)
    {
        return currBoard;
    }

    if (currBoard->tiles[currBoardPos].inUse)
    {
        return solutionRec(t, currTile, tileNum, currBoardPos + 1, currBoard);
    }

    int i, bestScore = -1, tmpScore;
    board_t *tmpB = makeBoard(currBoard->rows, currBoard->cols), *bestB = makeBoard(currBoard->rows, currBoard->cols);

    for (i = 0; i < tileNum; i++)
    {
        if (t[i].inUse)
        {
            continue;
        }

        t[i].inUse = true;
        currBoard->tiles[currBoardPos] = t[i];
        tmpB = solutionRec(t, currTile + 1, tileNum, currBoardPos + 1, currBoard);
        tmpScore = getScore(tmpB);
        if (tmpScore > bestScore)
        {
            bestScore = tmpScore;
            bestB = copyBoard(tmpB);
        }

        currBoard->tiles[currBoardPos] = rotateTile(&t[i]);
        tmpB = solutionRec(t, currTile + 1, tileNum, currBoardPos + 1, currBoard);
        tmpScore = getScore(tmpB);
        if (tmpScore > bestScore)
        {
            bestScore = tmpScore;
            bestB = copyBoard(tmpB);
        }

        t[i].inUse = false;
        currBoard->tiles[currBoardPos].inUse = false;
    }

    return bestB;
}