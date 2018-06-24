#include <stdio.h>
#include <ctype.h>

int can_print_it(char ch) {
  return isalpha(ch) || isblank(ch);
}

void print_letters(char arg[]) {
  for (int i = 0; arg[i] != '\0'; i++) {
    char ch = arg[i];

    if (can_print_it(ch)) {
      printf("'%c' == %d ", ch, ch);
    }
  }

  printf("\n");
}

void print_arguments(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    print_letters(argv[i]);
  }
}

int main(int argc, char *argv[]) {
  print_arguments(argc, argv);
  return 0;
}
