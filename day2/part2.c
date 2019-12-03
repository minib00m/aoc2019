#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct input_buffer
{
    int* buf;
    int capacity;
    int size;
};

static void init_input_buffer(struct input_buffer *ib, int capacity)
{
    ib->capacity = capacity;
    ib->size = 0;
    ib->buf = malloc(capacity * sizeof(int));
}

static void double_input_buffer(struct input_buffer *ib)
{
    ib->capacity *= 2;
    ib->buf = realloc(ib->buf, ib->capacity * sizeof(int));
}

static void destroy_input_buffer(struct input_buffer *ib)
{
    free(ib->buf);
}

static void clone_input_buffer(struct input_buffer *from, struct input_buffer *to)
{
    int cap_bytes = sizeof(int) * from->capacity;
    to->buf = malloc(cap_bytes);
    memcpy(to->buf, from->buf, cap_bytes);
    to->capacity = from->capacity;
    to->size = from->size;
}

static struct input_buffer read_stdin_input()
{
    struct input_buffer ib;
    init_input_buffer(&ib, 32);
    int n;
    while (scanf("%d%*c", &n) >= 1) {
        if (ib.size == ib.capacity) {
            double_input_buffer(&ib);
        }
        ib.buf[ib.size] = n;
        ib.size++;
    }
    return ib;
}

static void handle_add_opcode(struct input_buffer* ib, int idx)
{
    int pos1 = ib->buf[idx + 1];
    int pos2 = ib->buf[idx + 2];
    int res_pos = ib->buf[idx + 3];
    ib->buf[res_pos] = ib->buf[pos1] + ib->buf[pos2];
}

static void handle_mul_opcode(struct input_buffer* ib, int idx)
{
    int pos1 = ib->buf[idx + 1];
    int pos2 = ib->buf[idx + 2];
    int res_pos = ib->buf[idx + 3];
    ib->buf[res_pos] = ib->buf[pos1] * ib->buf[pos2];
}

static void adjust_input(struct input_buffer *ib, int pos1_val, int pos2_val)
{
    ib->buf[1] = pos1_val;
    ib->buf[2] = pos2_val;
}

int main()
{
    struct input_buffer input = read_stdin_input();

    int result_found = 0;
    int noun = 0;
    int verb = 0;

    for (int i = 0; i < 100 && !result_found; i++) {
        for (int j = 0; j < 100 && !result_found; j++) {
            struct input_buffer sim_input;
            clone_input_buffer(&input, &sim_input);
            adjust_input(&sim_input, i, j);
            int current_idx = 0;
            int halt_present = 0;
            int invalid_opcode = 0;
            while (!halt_present && !invalid_opcode) {
                if (current_idx >= sim_input.size) {
                    invalid_opcode = 1;
                    break;
                }
                switch (sim_input.buf[current_idx]) {
                    case 1:
                        handle_add_opcode(&sim_input, current_idx);
                        break;
                    case 2:
                        handle_mul_opcode(&sim_input, current_idx);
                        break;
                    case 99:
                        halt_present = 1;
                        break;
                    default:
                        invalid_opcode = 1;
                        break;
                }
                current_idx += 4;
            }

            if (halt_present && sim_input.buf[0] == 19690720) {
                result_found = 1;
                noun = i;
                verb = j;
            }
            destroy_input_buffer(&sim_input);
        }
    }
    destroy_input_buffer(&input);
    printf("%d\n", 100 * noun + verb);
}
