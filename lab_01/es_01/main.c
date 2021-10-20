#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_FILTRO 64
#define MAX_REGEXP 64
#define MAX_SRC 128
#define MAX_REGEXP_NUM 32

typedef enum
{
    Testo,
    Punto,
    Quadre,
    QuadreNegate,
    Minuscolo,
    Maiuscolo
} RegexpEnum;

typedef struct
{
    RegexpEnum exp;
    char filtro[MAX_FILTRO + 1];
} Regexp;

char *cercaRegexp(char *src, char *regexp);
char *regexpEnumToString(RegexpEnum rEnum);
size_t caricaRegexpStruct(Regexp *r, char *regexp);
bool validaFiltro(char lettera);

//per semplicità non ho usato l'allocazione dinamica. Tutte le lunghezze massime sono dichiarate sopra
int main()
{
    char src[MAX_SRC], regexp[MAX_REGEXP];
    printf("IN: src -> ");
    scanf("%s", src);
    printf("IN: regexp -> ");
    scanf("%s", regexp);

    char *ris = cercaRegexp(src, regexp);

    if (ris != NULL)
    {
        printf("OUT: primo match alla lettera -> '%c' ----- puntatore -> %p", *ris, ris);
    }
    else
    {
        printf("OUT: nessun match");
    }

    return 0;
}

char *cercaRegexp(char *src, char *regexp)
{
    Regexp r[MAX_REGEXP_NUM];
    size_t rLen = caricaRegexpStruct(r, regexp);
    size_t i, j, rIdx = 0, srcStartIdx;
    bool qFlag;

    for (i = 0; i < rLen; i++)
    {
        printf("%s\n", regexpEnumToString(r[i].exp));
    }

    //potrebbe matchare più caratteri, ma viene richiesta solo la prima. Appena matcha fa un return
    for (i = 0; i < strlen(src); i++)
    {
        if (rIdx == 0)
        {
            srcStartIdx = i;
        }

        switch (r[rIdx].exp)
        {
        case Testo:
            if ((i + strlen(r[rIdx].filtro) > strlen(src)))
            {
                i = strlen(src);
                break;
            }

            for (j = 0; j < strlen(r[rIdx].filtro); j++)
            {
                if (src[i + j] != r[rIdx].filtro[j])
                {
                    rIdx = 0;
                    break;
                }
                if (j == (strlen(r[rIdx].filtro) - 1))
                {
                    rIdx++;
                    i = i + j;
                }
            }
            break;

        case Punto:
            if (validaFiltro(src[i]))
            {
                rIdx++;
                break;
            }
            rIdx = 0;
            break;

        case Quadre:
            qFlag = false;
            for (j = 0; j < strlen(r[rIdx].filtro); j++)
            {
                if (src[i] == r[rIdx].filtro[j])
                {
                    qFlag = true;
                    break;
                }
            }
            if (qFlag)
            {
                rIdx++;
                break;
            }
            rIdx = 0;
            break;

        case QuadreNegate:
            qFlag = false;
            for (j = 0; j < strlen(r[rIdx].filtro); j++)
            {
                if (src[i] == r[rIdx].filtro[j])
                {
                    qFlag = true;
                    break;
                }
            }
            if (qFlag)
            {
                rIdx = 0;
                break;
            }
            rIdx++;
            break;

        case Minuscolo:
            if (islower(src[i]))
            {
                rIdx++;
                break;
            }
            rIdx = 0;
            break;

        case Maiuscolo:
            if (islower(src[i]))
            {
                rIdx = 0;
                break;
            }
            rIdx++;
            break;

        default:
            break;
        }

        if (rIdx == rLen)
        {
            printf("INFO: match tra gli indici [%d:%d]\n", srcStartIdx, i);
            rIdx = 0;

            return &src[srcStartIdx];
        }
    }

    return NULL;
}

bool validaFiltro(char lettera)
{
    return (lettera >= 'A' && lettera <= 'Z') || (lettera >= 'a' && lettera <= 'z') || (lettera >= '0' && lettera <= '9');
}

size_t caricaRegexpStruct(Regexp *r, char *regexp)
{
    size_t i, j, filCont, rIdx = 0;
    for (i = 0; i < strlen(regexp); i++)
    {
        if (rIdx == MAX_REGEXP_NUM)
        {
            break;
        }

        if (regexp[i] == '.')
        {
            r[rIdx].exp = Punto;
        }
        else if (regexp[i] == 92)
        {
            if (regexp[i + 1] == 'a')
            {
                r[rIdx].exp = Minuscolo;
            }
            else if (regexp[i + 1] == 'A')
            {
                r[rIdx].exp = Maiuscolo;
            }
            else
            {
                printf("ERRORE: '\' deve essere seguita da 'A' o 'a'");
                exit(3);
            }
            i++;
        }
        else if (regexp[i] == '[')
        {
            if (regexp[i + 1] == '^')
            {
                r[rIdx].exp = QuadreNegate;
                j = i + 2;
            }
            else
            {
                r[rIdx].exp = Quadre;
                j = i + 1;
            }
            char filtro[MAX_FILTRO + 1] = "";
            filCont = 0;
            while (regexp[j] != ']' && filCont < MAX_FILTRO)
            {
                if (j == strlen(regexp))
                {
                    printf("ERRORE: '[' non chiusa");
                    exit(2);
                }
                if (!validaFiltro(regexp[j]))
                {
                    printf("ERRORE: '%c' non supportato (solo alfanumerici)", regexp[j]);
                    exit(1);
                }
                strncat(filtro, &regexp[j], 1);
                j++;
                filCont++;
            }
            filtro[filCont] = '\0';
            strcpy(r[rIdx].filtro, filtro);
            i = j;
        }
        else
        {
            r[rIdx].exp = Testo;
            j = i;
            char filtro[MAX_FILTRO + 1] = "";
            filCont = 0;
            while (validaFiltro(regexp[j]) && filCont < MAX_FILTRO)
            {
                if (j == strlen(regexp))
                {
                    break;
                }
                if (!validaFiltro(regexp[j]))
                {
                    printf("ERRORE: '%c' non supportato (solo alfanumerici)", regexp[j]);
                    exit(1);
                }
                strncat(filtro, &regexp[j], 1);
                j++;
                filCont++;
            }
            filtro[filCont] = '\0';
            strcpy(r[rIdx].filtro, filtro);
            i = j - 1;
        }

        if (strlen(r[rIdx].filtro) > 0)
        {
            printf("INFO: [%d] regexp: %s ----- filtro: %s\n", rIdx, regexpEnumToString(r[rIdx].exp), r[rIdx].filtro);
        }
        else
        {
            printf("INFO: [%d] regexp: %s\n", rIdx, regexpEnumToString(r[rIdx].exp));
        }
        rIdx++;
    }

    return rIdx;
}

char *regexpEnumToString(RegexpEnum rEnum)
{
    switch (rEnum)
    {
    case Testo:
        return "Testo -> 'abcd'";
    case Punto:
        return "Punto -> '.'";
    case Quadre:
        return "Quadre -> '[]'";
    case QuadreNegate:
        return "Quadre Negate -> '[^]'";
    case Minuscolo:
        return "Minuscolo -> '/a'";
    case Maiuscolo:
        return "Maiuscolo -> '/A'";
    default:
        return "";
    }
}
