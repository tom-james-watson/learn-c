#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE 5

struct Stack {
  int entries[STACK_SIZE];
  int *pos;
};

void push(struct Stack *stack, int i) {
  printf("> push %d\n", i);

  if (stack->pos - stack->entries >= STACK_SIZE) {
    printf("stack overflow\n");
    return;
  }

  *stack->pos = i;
  stack->pos++;
}

void pop(struct Stack *stack) {
  printf("> pop\n");

  if (stack->pos - stack->entries == 0) {
    printf("stack underflow\n");
    return;
  }

  stack->pos--;
  int i = *stack->pos;
  printf("popped %d\n", i);
}

int peek(struct Stack *stack);

void is_empty(struct Stack *stack) {
  printf("> isempty\n");

  if (stack->pos == stack->entries) {
    printf("empty\n");
  } else {
    printf("not empty\n");
  }
}

struct Stack *Create_stack() {
  struct Stack *stack = malloc(sizeof(struct Stack));
  stack->pos = stack->entries;

  return stack;
}

void print(struct Stack *stack) {
  printf("> print\n");

  for (int *cur = stack->entries; cur < stack->pos; cur++) {
    printf("(%p) = %d\n", cur, *cur);
  }
}

int main(void) {
  struct Stack *stack = Create_stack();

  push(stack, 12);
  push(stack, 21);
  push(stack, 148);
  push(stack, 4);
  push(stack, 1000);
  print(stack);
  push(stack, 27);
  print(stack);
  pop(stack);
  print(stack);
  pop(stack);
  pop(stack);
  pop(stack);
  print(stack);
  is_empty(stack);
  pop(stack);
  is_empty(stack);
  pop(stack);
  print(stack);

  return 0;
}
