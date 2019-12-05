#include <stdlib.h>
#include "line.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static int is_horizontal(struct line *l)
{
    return l->p1.y == l->p2.y;
}

static int is_vertical(struct line *l)
{
    return l->p1.x == l->p2.x;
}

static int is_between(int bound1, int bound2, int query)
{
    return bound1 < query && query < bound2;
}

int line_intersect(struct line *l1, struct line *l2, struct point *res)
{
    if ((is_horizontal(l1) && is_horizontal(l2)) || (is_vertical(l1) && is_vertical(l2))) {
        return 0;
    }
    if (!is_horizontal(l1)) {
        struct line *tmp = l1;
        l1 = l2;
        l2 = tmp;
    }
    const int l2_min_y = MIN(l2->p1.y, l2->p2.y);
    const int l2_max_y = MAX(l2->p1.y, l2->p2.y);
    const int l2_x = l2->p1.x;
    const int l1_min_x = MIN(l1->p1.x, l1->p2.x);
    const int l1_max_x = MAX(l1->p1.x, l1->p2.x);
    const int l1_y = l1->p1.y;
    if (is_between(l2_min_y, l2_max_y, l1_y) && is_between(l1_min_x, l1_max_x, l2_x)) {
        res->x = l2_x;
        res->y = l1_y;
        return 1;
    }
    return 0;
}

int line_length(struct line *l)
{
    if (is_vertical(l)) {
        return abs(l->p1.y - l->p2.y);
    }
    return abs(l->p1.x - l->p2.x);
}
