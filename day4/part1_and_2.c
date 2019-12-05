#include <stdio.h>

static int is_good_password_part1(char arr[static 6])
{
    int has_doubles = 0;
    for (int i = 1; i < 6; i++) {
        if (arr[i - 1] == arr[i]) {
            has_doubles = 1;
        }
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }
    return has_doubles;
}

static int is_good_password_part2(char arr[static 6])
{
    for (int i = 1; i < 6; i++) {
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }

    int same = 1;
    for (int i = 1; i < 6; i++) {
        if (arr[i - 1] == arr[i]) {
            same++;
        } else if (same == 2) {
            return 1;
        } else {
            same = 1;
        }
    }
    return same == 2;
}

static int num_of_matching_passwords(int up_to, int(*matcher)(char arr[static 6]))
{
    char arr[16];
    int result = 0;
    for (int i = 0; i < up_to; i++) {
        sprintf(arr, "%06d", i);
        result += matcher(arr);
    }
    return result;
}

int main()
{
    int from, to;
    scanf("%d%d", &from, &to);
    const int p1res1 = num_of_matching_passwords(from, is_good_password_part1);
    const int p1res2 = num_of_matching_passwords(to, is_good_password_part1);
    puts("PART1");
    printf("%d\n", p1res2 - p1res1);

    const int p2res1 = num_of_matching_passwords(from, is_good_password_part2);
    const int p2res2 = num_of_matching_passwords(to, is_good_password_part2);
    puts("PART2");
    printf("%d\n", p2res2 - p2res1);
}
