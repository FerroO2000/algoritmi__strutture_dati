#include <stdlib.h>
#include <stdio.h>

#define EXIT_NUM -1

int majority(int *a, int N);
int majorityRec(int *a, int start, int end);

int main()
{
    int vet[] = {0, 1, 0, 2, 3, 4, 0, 5}, N = 8;

    printf("maggioritario: %d", majority(vet, N));

    return 0;
}

int majorityRec(int *a, int start, int end)
{
    if (start == end)
    {
        return a[start];
    }

    int mid = ((end - start) / 2) + start + 1;

    int left = majorityRec(a, start, mid - 1);
    int right = majorityRec(a, mid, end);

    if (left == right)
    {
        return left;
    }

    int len = (end - start) + 1, i, lCount = 0, rCount = 0;
    for (i = start; i <= end; i++)
    {
        if (left != EXIT_NUM && a[i] == left)
        {
            lCount++;
            continue;
        }
        if (right != EXIT_NUM && a[i] == right)
        {
            rCount++;
        }
    }
    if (lCount > len / 2)
    {
        return left;
    }
    if (rCount > len / 2)
    {
        return right;
    }

    return EXIT_NUM;
}

int majority(int *a, int N)
{
    return majorityRec(a, 0, N - 1);
}