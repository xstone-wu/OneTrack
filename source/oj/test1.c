#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int count;
    char ch;
} ChNode;

int cmp(const void *a, const void *b)
{
    return *(char *)a - *(char *)b;
}

int cmpNode(const void *a, const void *b)
{
    return ((ChNode *)b)->count - ((ChNode *)a)->count;
}

int main()
{
    char str[1024] = {0};
    gets(str);
    printf("str=%s\n", str);

    qsort(str, strlen(str), sizeof(str[0]), cmp);
    printf("sortStr=%s\n", str);

    char bigStr[1024] = {0};
    char smallStr[1024] = {0};
    if (str[0] >= 'A' && str[0] <= 'Z') {
        sscanf(str, "%[A-Z]%s", bigStr, smallStr);
    } else {
        sscanf(str, "%[a-z]", smallStr);
    }

    printf("bigStr=%s, smallStr=%s\n", bigStr, smallStr);

    char *pSmallStr = smallStr;
    char *pBigStr = bigStr;
    ChNode chNode[68] = {0};

    char tmp = *pSmallStr;
    int i = 0;
    while (*pSmallStr != '\0') {
        if (tmp != *pSmallStr) {
            i++;
        }

        chNode[i].count++;
        chNode[i].ch = tmp;

        tmp = *pSmallStr;
        pSmallStr++;
    }

    i++;
    tmp = *pBigStr;
    while (*pBigStr != '\0') {
        if (tmp != *pBigStr) {
            i++;
        }

        chNode[i].count++;
        chNode[i].ch = tmp;

        tmp = *pBigStr;
        pBigStr++;
    }

    qsort(chNode, 68, sizeof(ChNode), cmpNode);

    for (i = 0; chNode[i].count != 0; i++) {
        printf("%c:%d", chNode[i].ch, chNode[i].count);
        if (chNode[i + 1].count != 0) {
            printf(";");
        }
    }

    printf("\n");
    return 0;
}