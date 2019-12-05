#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "line.h"

struct line_array
{
    struct line *buf;
    size_t size;
    size_t capacity;
};

static void line_array_init(struct line_array *la, size_t capacity)
{
    la->buf = malloc(capacity * sizeof(*la->buf));
    la->size = 0;
    la->capacity = capacity;
}

static void line_array_destroy(struct line_array *la)
{
    free(la->buf);
}

static void line_array_push_back(struct line_array *la, struct line l)
{
    if (la->size == la->capacity) {
        la->capacity *= 2;
        la->buf = realloc(la->buf, la->capacity * sizeof(*la->buf));
    }
    la->buf[la->size] = l;
    la->size++;
}

static struct line_array read_line_description()
{
    struct line_array input;
    struct point prev_p = {0, 0};
    struct point next_p;
    line_array_init(&input, 32);
    char direction;
    int count;
    while (scanf("%c%d", &direction, &count) == 2) {
        switch (direction) {
            case 'U':
                next_p = (struct point){.x = prev_p.x, .y = prev_p.y + count};
                break;
            case 'D':
                next_p = (struct point){.x = prev_p.x, .y = prev_p.y - count};
                break;
            case 'L':
                next_p = (struct point){.x = prev_p.x - count, .y = prev_p.y};
                break;
            case 'R':
                next_p = (struct point){.x = prev_p.x + count, .y = prev_p.y};
                break;
            default:
                __builtin_unreachable();
        }
        line_array_push_back(&input, (struct line){.p1 = prev_p, .p2 = next_p});
        prev_p = next_p;

        char delim = 0;
        scanf("%c", &delim);
        if (delim != ',') {
            break;
        }
    }

    return input;
}

static int calculate_min_manhattan_distance(struct line_array *la1, struct line_array *la2)
{
    int result = INT_MAX;
    for (size_t i = 0; i < la1->size; i++) {
        for (size_t j = 0; j < la2->size; j++) {
            struct point isect;
            if (line_intersect(la1->buf + i, la2->buf + j, &isect) > 0) {
                int distance = abs(isect.x) + abs(isect.y);
                result = distance < result ? distance : result;
            }
        }
    }
    return result;
}

static struct line construct_semi_line(struct line *l, struct point *p)
{
    return (struct line){.p1 = l->p1, .p2 = *p};
}

static int calculate_min_wire_length(struct line_array *la1, struct line_array *la2)
{
    int fst_len = 0;
    int snd_len = 0;
    int result = INT_MAX;
    for (size_t i = 0; i < la1->size; i++) {
        for (size_t j = 0; j < la2->size; j++) {
            if (j == 0) {
                snd_len = 0;
            }
            struct point isect;
            if (line_intersect(la1->buf + i, la2->buf + j, &isect) > 0) {
                struct line semi_l1 = construct_semi_line(la1->buf + i, &isect);
                struct line semi_l2 = construct_semi_line(la2->buf + j, &isect);
                const int semi_dist = line_length(&semi_l1) + line_length(&semi_l2);
                const int distance = fst_len + snd_len + semi_dist;
                result = distance < result ? distance : result;
            }
            snd_len += line_length(la2->buf + j);
        }
        fst_len += line_length(la1->buf + i);
    }
    return result;
}

int main()
{
    struct line_array fst_line_arr = read_line_description();
    struct line_array snd_line_arr = read_line_description();

    int manhattan_distance = calculate_min_manhattan_distance(&fst_line_arr, &snd_line_arr);
    puts("PART1");
    printf("%d\n", manhattan_distance);
    int min_wire_length = calculate_min_wire_length(&fst_line_arr, &snd_line_arr);
    puts("PART2");
    printf("%d\n", min_wire_length);

    line_array_destroy(&fst_line_arr);
    line_array_destroy(&snd_line_arr);
}
