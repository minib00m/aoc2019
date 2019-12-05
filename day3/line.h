struct point
{
    int x;
    int y;
};

struct line
{
    struct point p1;
    struct point p2;
};

int line_intersect(struct line *l1, struct line *l2, struct point *res);
int line_length(struct line *l);
