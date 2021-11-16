#include <stdio.h>
#include <stdlib.h>

#define GEM_NUM

typedef enum
{
    sapphire,
    ruby,
    topaz,
    emerald,
} gems_e;

int main()
{
    int gems[GEM_NUM] = {5, 7, 4, 10};
    gems_e sol[100];

    int a = f(0, 26, sol, gems, 0);

    printf("%d", a);

    return 0;
}

int checkSol(int *sol, int sol_dim)
{
    if ((sol[sol_dim - 1] == sapphire && !(sol[0] == sapphire || sol[0] == ruby)) ||
        (sol[sol_dim - 1] == emerald && !(sol[0] == emerald || sol[0] == topaz)) ||
        (sol[sol_dim - 1] == ruby && !(sol[0] == emerald || sol[0] == topaz)) ||
        (sol[sol_dim - 1] == topaz && !(sol[0] == sapphire || sol[0] == ruby)))
    {
        return 0;
    }

    return 1;
}

int f(int currPos, int maxDim, gems_e *sol, int val[GEM_NUM], int currLen)
{
    if (currPos >= maxDim)
    {
        printf("trovata massima\n");
        for (int i = 0; i < maxDim; i++)
        {
            printf("%d - ", sol[i]);
        }
        return maxDim;
    }

    if (currPos + 1 > currLen && checkSol(sol, currPos))
        currLen = currPos + 1;

    if (currPos == 0)
    {
        for (gems_e g = sapphire; g <= emerald; g++)
        {
            sol[currPos] = g;
            val[g]--;
            currLen = f(++currPos, maxDim, sol, val, currLen);
            if (currLen == maxDim)
            {
                return currLen;
            }
            val[g]++;
        }
    }
    else
    {
        switch (sol[currPos - 1])
        {
        case sapphire:
        case topaz:
            if (val[sapphire] > 0)
            {
                sol[currPos] = sapphire;
                val[sapphire]--;
                currLen = f(++currPos, maxDim, sol, val, currLen);
                if (currLen == maxDim)
                {
                    return currLen;
                }
                val[sapphire]++;
            }

            if (val[ruby] > 0)
            {
                sol[currPos] = ruby;
                val[ruby]--;
                currLen = f(++currPos, maxDim, sol, val, currLen);
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
                currLen = f(++currPos, maxDim, sol, val, currLen);
                if (currLen == maxDim)
                {
                    return currLen;
                }
                val[emerald]++;
            }

            if (val[topaz] > 0)
            {
                sol[currPos] = topaz;
                val[topaz]--;
                currLen = f(++currPos, maxDim, sol, val, currLen);
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
    }

    return currLen;
}
