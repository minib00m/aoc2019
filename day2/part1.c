#include <stdio.h>
#include <stdlib.h>

static int *read_stdin_input()
{
    size_t elems = 32;
    size_t idx = 0;
    int *input = malloc(sizeof(int) * elems);
    int n;
    while (scanf("%d%*c", &n) >= 1) {
        if (idx == elems) {
            elems *= 2;
            input = realloc(input, sizeof(int) * elems);
        }
        input[idx] = n;
        idx++;
    }
    return input;
}

static void handle_add_opcode(int *input, int idx)
{
    int pos1 = input[idx + 1];
    int pos2 = input[idx + 2];
    int res_pos = input[idx + 3];
    input[res_pos] = input[pos1] + input[pos2];
}

static void handle_mul_opcode(int *input, int idx)
{
    int pos1 = input[idx + 1];
    int pos2 = input[idx + 2];
    int res_pos = input[idx + 3];
    input[res_pos] = input[pos1] * input[pos2];
}

int main()
{
    int *input = read_stdin_input();
    input[1] = 12;
    input[2] = 2;
    int current_idx = 0;
    int halt_present = 0;
    while (!halt_present) {
        switch (input[current_idx]) {
            case 1:
                handle_add_opcode(input, current_idx);
                break;
            case 2:
                handle_mul_opcode(input, current_idx);
                break;
            case 99:
                halt_present = 1;
                break;
            default:
                __builtin_unreachable();
        }
        current_idx += 4;
    }
    printf("%d\n", input[0]);
    free(input);
}
