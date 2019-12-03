#include "line.h"

static int is_horizontal(struct line *l)
{
    return l->p1.y == l->p2.y;
}

static int is_vertical(struct line *l)
{
    return l->p1.x == l->p2.x;
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
    if (l1->p1.y >= l2->p1.y && l1->p1.y <= l2->p2.y) {
        res->x = l2->p1.x;
        res->y = l1->p1.y;
        return 1;
    }
    return 0;
}
