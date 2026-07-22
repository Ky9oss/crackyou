#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *a, const void *b)
{
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcmp(s1, s2);
}

int main(void)
{
    char *words[] = {
        "banana",
        "apple",
        "orange",
        "grape"
    };

    int n = sizeof(words) / sizeof(words[0]);
    // printf("%ld %ld %d\n", sizeof(words), sizeof(words[0]), n);

    qsort(words, n, sizeof(char *), cmp);

    for (int i = 0; i < n; i++)
        printf("%s\n", words[i]);

    return 0;
}
