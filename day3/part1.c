#include <stdio.h>
#include "line.h"

int main()
{
    struct line l1 = {{0, 0}, {10, 0}};
    struct line l2 = {{0, 2}, {0, 10}};

    struct point res = {-1, -1};

    int res_i = line_intersect(&l1, &l2, &res);
    printf("%d\n", res_i);
    printf("%d %d\n", res.x, res.y);
}
