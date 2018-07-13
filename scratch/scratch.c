#include <stdio.h>

typedef int (*operation)(int, int);

int add(int x, int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

// Another function that accepts a pointer to a function with the same
// prototype as add/subtract
int calculate(operation op, int x, int y) {
  return op(x, y);
}

int main(int argc, char *argv[]) {
  printf("%d\n", calculate(add, 6, 3)); // 9
  printf("%d\n", calculate(subtract, 6, 3)); // 3
}
