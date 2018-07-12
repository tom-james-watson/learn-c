#include <stdio.h>

int main(int argc, char *argv[]) {
  int val = 1;
  int *ptr = 0;

  // declare a variable ptr2ptr which holds the value-at-address of
  // an *int type which in holds the value-at-address of an int type
  int **ptr2ptr = 0;
  ptr = &val;
  ptr2ptr = &ptr;

  printf("&ptr=%p, &val=%p\n", (void *)&ptr, (void *)&val);
  // &ptr=0x7fff390fa6f8, &val=0x7fff390fa70c

  printf(
    "ptr2ptr=%p, *ptr2ptr=%p, **ptr2ptr=%d\n",
    ptr2ptr,
    *ptr2ptr,
    **ptr2ptr
  );
}
