#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define ARRAY_SIZE(a) sizeof(a)/sizeof(*a)

enum OPCODE
{
    OPCODE_ADD = 1,
    OPCODE_MUL = 2,
    OPCODE_INPUT = 3,
    OPCODE_OUTPUT = 4,
    OPCODE_JE = 5,
    OPCODE_JN = 6,
    OPCODE_SETL = 7,
    OPCODE_SETEQ = 8,

    OPCODE_HALT = 99
};

enum MODE
{
    POSITION = 0,
    IMMEDIATE = 1
};

struct instruction_mode
{
    int instruction;
    enum MODE mode[3];
};

struct input_array
{
    int *buf;
    size_t size;
    size_t capacity;
};

static void input_array_init(struct input_array *ia, size_t capacity)
{
    ia->buf = malloc(capacity * sizeof(*ia->buf));
    ia->size = 0;
    ia->capacity = capacity;
}

static void input_array_destroy(struct input_array *ia)
{
    free(ia->buf);
}

static void input_array_double(struct input_array *ia)
{
    ia->capacity *= 2;
    ia->buf = realloc(ia->buf, ia->capacity * sizeof(*ia->buf));
}

static void input_array_push_back(struct input_array *ia, int val)
{
    if (ia->size == ia->capacity) {
        input_array_double(ia);
    }
    ia->buf[ia->size] = val;
    ia->size++;
}

static struct input_array read_input(char* filename)
{
    char ch;
    char num[32];
    int num_len = 0;
    struct input_array ia;
    int n;
    input_array_init(&ia, 32);
    FILE *fh = fopen(filename, "r");
    while ((ch = getc(fh)) != EOF) {
        if (isdigit(ch) || ch == '-') {
            num[num_len] = ch;
            num_len++;
        } else {
            if (num_len > 0) {
                num[num_len] = 0;
                sscanf(num, "%d", &n);
                input_array_push_back(&ia, n);
                num_len = 0;
            }
        }
    }
    if (num_len > 0) {
        num[num_len] = 0;
        sscanf(num, "%d", &n);
        input_array_push_back(&ia, n);
    }
    return ia;
}

static int *position_or_immediate(int *input, int idx, enum MODE mode)
{
    if (mode == POSITION) {
        int pos = input[idx];
        return input + pos;
    }
    return input + idx;
}

static void handle_opcode_add(int *input, int idx, enum MODE *mode)
{
    int lhs = *position_or_immediate(input, idx + 1, mode[0]);
    int rhs = *position_or_immediate(input, idx + 2, mode[1]);
    int *res = position_or_immediate(input, idx + 3, mode[2]);
    *res = lhs + rhs;
}

static void handle_opcode_mul(int *input, int idx, enum MODE *mode)
{
    int lhs = *position_or_immediate(input, idx + 1, mode[0]);
    int rhs = *position_or_immediate(input, idx + 2, mode[1]);
    int *res = position_or_immediate(input, idx + 3, mode[2]);
    *res = lhs * rhs;
}

static void handle_opcode_input(int *input, int idx, __attribute__((unused)) enum MODE *mode)
{
    char num[32];
    int num_len = 0;
    char ch;
    while ((ch = getchar()) != EOF) {
        if (!isdigit(ch) && ch != '-') {
            break;
        }
        num[num_len] = ch;
        num_len++;
    }
    num[num_len] = 0;
    int pos = input[idx + 1];
    sscanf(num, "%d", input + pos);
}

static void handle_opcode_output(int *input, int idx, enum MODE* mode)
{
    int value = *position_or_immediate(input, idx + 1, mode[0]);
    printf("%d\n", value);
}

static int handle_opcode_je(int *input, int idx, enum MODE *mode)
{
    int value = *position_or_immediate(input, idx + 1, mode[0]);
    int dest = *position_or_immediate(input, idx + 2, mode[1]);
    return value != 0 ? dest : -1;
}

static int handle_opcode_jn(int *input, int idx, enum MODE *mode)
{
    int value = *position_or_immediate(input, idx + 1, mode[0]);
    int dest = *position_or_immediate(input, idx + 2, mode[1]);
    return value == 0 ? dest : -1;
}

static void handle_opcode_setl(int *input, int idx, enum MODE *mode)
{
    int lhs = *position_or_immediate(input, idx + 1, mode[0]);
    int rhs = *position_or_immediate(input, idx + 2, mode[1]);
    int *dest = position_or_immediate(input, idx + 3, mode[2]);
    *dest = lhs < rhs;
}

static void handle_opcode_seteq(int *input, int idx, enum MODE *mode)
{
    int lhs = *position_or_immediate(input, idx + 1, mode[0]);
    int rhs = *position_or_immediate(input, idx + 2, mode[1]);
    int *dest = position_or_immediate(input, idx + 3, mode[2]);
    *dest = lhs == rhs;
}

static int instruction_length(enum OPCODE oc)
{
    switch (oc) {
        case OPCODE_ADD:
        case OPCODE_MUL:
        case OPCODE_SETL:
        case OPCODE_SETEQ:
            return 4;
        case OPCODE_INPUT:
        case OPCODE_OUTPUT:
            return 2;
        case OPCODE_JE:
        case OPCODE_JN:
            return 3;
        case OPCODE_HALT:
            return 1;
    }
    return 0;
}

static void parse_instruction_mode(int value, struct instruction_mode *im)
{
    im->instruction = value % 100;
    value /= 100;
    for (int i = 0; i < 3; i++) {
        im->mode[i] = value % 10;
        value /= 10;
    }
}

int main(int argc, char **argv)
{
    struct input_array input = read_input(argv[1]);
    int current_idx = 0;
    int halt_present = 0;
    struct instruction_mode im;
    while (!halt_present) {
        parse_instruction_mode(input.buf[current_idx], &im);
        switch (im.instruction) {
            case OPCODE_ADD:
                handle_opcode_add(input.buf, current_idx, im.mode);
                break;
            case OPCODE_MUL:
                handle_opcode_mul(input.buf, current_idx, im.mode);
                break;
            case OPCODE_INPUT:
                handle_opcode_input(input.buf, current_idx, im.mode);
                break;
            case OPCODE_OUTPUT:
                handle_opcode_output(input.buf, current_idx, im.mode);
                break;
            case OPCODE_JE:
            {
                int new_idx = handle_opcode_je(input.buf, current_idx, im.mode);
                if (new_idx != -1) {
                    current_idx = new_idx;
                    continue;
                }
                break;
            }
            case OPCODE_JN:
            {
                int new_idx = handle_opcode_jn(input.buf, current_idx, im.mode);
                if (new_idx != -1) {
                    current_idx = new_idx;
                    continue;
                }
                break;
            }
            case OPCODE_SETL:
                handle_opcode_setl(input.buf, current_idx, im.mode);
                break;
            case OPCODE_SETEQ:
                handle_opcode_seteq(input.buf, current_idx, im.mode);
                break;
            case OPCODE_HALT:
                halt_present = 1;
                break;
            default:
                printf("WRONG OPCODE: %d\n", im.instruction);
                return EXIT_FAILURE;
        }
        current_idx += instruction_length(im.instruction);
    }
    input_array_destroy(&input);
}
