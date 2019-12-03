#include <stdio.h>

int main()
{
    int n;
    int sum = 0;
    while (scanf("%d", &n) == 1) {
        int module_sum = 0;
        while (n / 3 - 2 > 0) {
            module_sum += n / 3 - 2;
            n = n / 3 - 2;
        }
        sum += module_sum;
    }
    printf("%d\n", sum);
}
