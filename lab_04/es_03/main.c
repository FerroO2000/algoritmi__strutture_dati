#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "hard_test_set.txt"
#define GEM_NUM 4

typedef enum
{
    sapphire,
    ruby,
    topaz,
    emerald,
} gems_e;

int maxNecklace(int val[GEM_NUM]);
char getGemChar(gems_e g);
int recursive(int currPos, int maxDim, gems_e *sol, int val[GEM_NUM], int currLen);

int main()
{
    FILE *fp = fopen(INPUT_FILE, "r");
    if (fp == NULL)
    {
        return 1;
    }

    int gems[GEM_NUM], n, i;
    fscanf(fp, "%d\n", &n);

    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d %d %d %d\n", &gems[sapphire], &gems[ruby], &gems[topaz], &gems[emerald]);
        printf("zaffiri: %d\nrubini: %d\ntopazi: %d\nsmeraldi: %d\n", gems[sapphire], gems[ruby], gems[topaz], gems[emerald]);
        printf("\nlunghezza massima: %d\n\n", maxNecklace(gems));
    }

    fclose(fp);

    return 0;
}

int maxNecklace(int val[GEM_NUM])
{
    int maxDim = val[sapphire] + val[ruby] + val[topaz] + val[emerald], bestLen = 0, lastLen, i;
    gems_e tmpSol[maxDim], *sol;

    for (gems_e g = sapphire; g <= emerald; g++)
    {
        tmpSol[0] = g;
        val[g]--;
        lastLen = recursive(1, maxDim, tmpSol, val, 0);
        if (lastLen == maxDim)
        {
            bestLen = maxDim;
            sol = (gems_e *)calloc(bestLen, sizeof(gems_e));
            for (i = 0; i < bestLen; i++)
            {
                sol[i] = tmpSol[i];
            }
            break;
        }
        if (lastLen > bestLen)
        {
            bestLen = lastLen;
            sol = (gems_e *)calloc(bestLen, sizeof(gems_e));
            for (i = 0; i < bestLen; i++)
            {
                sol[i] = tmpSol[i];
            }
        }
        val[g]++;
    }

    for (i = 0; i < bestLen; i++)
    {
        if (i == 0)
        {
            printf("composizione: %c", getGemChar(sol[i]));
        }
        else
        {
            printf(" - %c", getGemChar(sol[i]));
        }
    }

    free(sol);

    return bestLen;
}

char getGemChar(gems_e g)
{
    switch (g)
    {
    case sapphire:
        return 'Z';
    case ruby:
        return 'R';
    case topaz:
        return 'T';
    case emerald:
        return 'S';
    default:
        return 0;
    }
}

int recursive(int currPos, int maxDim, gems_e *sol, int val[GEM_NUM], int currLen)
{
    if (currPos >= maxDim)
    {
        return maxDim;
    }

    if (currPos > currLen)
    {
        currLen = currPos;
    }

    switch (sol[currPos - 1])
    {
    case sapphire:
    case topaz:
        if (val[sapphire] > 0)
        {
            sol[currPos] = sapphire;
            val[sapphire]--;
            currLen = recursive(++currPos, maxDim, sol, val, currLen);
            if (currLen == maxDim)
            {
                return currLen;
            }
            val[sapphire]++;
            break;
        }

        if (val[ruby] > 0)
        {
            sol[currPos] = ruby;
            val[ruby]--;
            currLen = recursive(++currPos, maxDim, sol, val, currLen);
            if (currLen == maxDim)
            {
                return currLen;
            }
            val[ruby]++;
        }

        break;

    case emerald:
    case ruby:
        if (val[emerald] > 0)
        {
            sol[currPos] = emerald;
            val[emerald]--;
            currLen = recursive(++currPos, maxDim, sol, val, currLen);
            if (currLen == maxDim)
            {
                return currLen;
            }
            val[emerald]++;
            break;
        }

        if (val[topaz] > 0)
        {
            sol[currPos] = topaz;
            val[topaz]--;
            currLen = recursive(++currPos, maxDim, sol, val, currLen);
            if (currLen == maxDim)
            {
                return currLen;
            }
            val[topaz]++;
        }

        break;

    default:
        break;
    }

    return currLen;
}

// int recursive(int currPos, int maxDim, gems_e *sol, int val[GEM_NUM], int currLen)
// {
//     if (currPos >= maxDim)
//     {
//         printf("trovata massima\n");
//         for (int i = 0; i < maxDim; i++)
//         {
//             printf("%d - ", sol[i]);
//         }
//         return maxDim;
//     }

//     /*if (currPos + 1 > currLen && checkSol(sol, currPos))
//         currLen = currPos + 1;*/
//     if (currPos > currLen)
//     {
//         currLen = currPos;
//     }

//     if (currPos == 0)
//     {
//         for (gems_e g = sapphire; g <= emerald; g++)
//         {
//             sol[0] = g;
//             val[g]--;
//             currLen = f(1, maxDim, sol, val, 0);
//             printf("%d : %d\n", g, currLen);
//             if (currLen == maxDim)
//             {
//                 return currLen;
//             }
//             val[g]++;
//             /*currLen = 0;
//             currPos = 0;*/
//         }
//     }
//     else
//     {
//         switch (sol[currPos - 1])
//         {
//         case sapphire:
//         case topaz:
//             if (val[sapphire] > 0)
//             {
//                 sol[currPos] = sapphire;
//                 val[sapphire]--;
//                 currLen = f(++currPos, maxDim, sol, val, currLen);
//                 if (currLen == maxDim)
//                 {
//                     return currLen;
//                 }
//                 val[sapphire]++;
//                 break;
//             }

//             if (val[ruby] > 0)
//             {
//                 sol[currPos] = ruby;
//                 val[ruby]--;
//                 currLen = f(++currPos, maxDim, sol, val, currLen);
//                 if (currLen == maxDim)
//                 {
//                     return currLen;
//                 }
//                 val[ruby]++;
//             }

//             break;

//         case emerald:
//         case ruby:
//             if (val[emerald] > 0)
//             {
//                 sol[currPos] = emerald;
//                 val[emerald]--;
//                 currLen = f(++currPos, maxDim, sol, val, currLen);
//                 if (currLen == maxDim)
//                 {
//                     return currLen;
//                 }
//                 val[emerald]++;
//                 break;
//             }

//             if (val[topaz] > 0)
//             {
//                 sol[currPos] = topaz;
//                 val[topaz]--;
//                 currLen = f(++currPos, maxDim, sol, val, currLen);
//                 if (currLen == maxDim)
//                 {
//                     return currLen;
//                 }
//                 val[topaz]++;
//             }

//             break;

//         default:
//             break;
//         }
//     }

//     return currLen;
// }
