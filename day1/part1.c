#include <stdio.h>

int main()
{
    int n;
    int sum = 0;
    while (scanf("%d", &n) == 1) {
        sum += n / 3 - 2;
    }
    printf("%d\n", sum);
}
