#include <stdlib.h>
#include <stdio.h>

int gcd(int a, int b);

int main()
{
    int a, b;
    printf("a: ");
    scanf("%d", &a);
    printf("b: ");
    scanf("%d", &b);

    printf("gcd: %d", gcd(a, b));

    return 0;
}

int gcd(int a, int b)
{
    int tmp;
    if (b > a)
    {
        tmp = a;
        a = b;
        b = tmp;
    }

    if (b == a)
    {
        return a;
    }

    int aR = a % 2, bR = b % 2;

    if (aR == 0 && bR == 0)
    {
        return 2 * gcd(a / 2, b / 2);
    }
    if (bR == 0 && aR != 0)
    {
        return gcd(a, b / 2);
    }
    if (bR != 0 && aR != 0)
    {
        return gcd((a - b) / 2, b);
    }
}