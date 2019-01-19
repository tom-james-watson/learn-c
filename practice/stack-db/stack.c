#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define STACK_SIZE 10

struct StackFile {
  struct Stack *stack;
  FILE *file;
};

struct Stack {
  int entries[STACK_SIZE];
  int pos;
};

void die(char *err) {
  printf("%s\n", err);
  exit(1);
}

struct StackFile *Stackfile_open(char *filename, char *action) {
  struct StackFile *stackfile = malloc(sizeof(struct StackFile));
  if (!stackfile) {
    die("Memory error");
  }

  stackfile->stack = malloc(sizeof(struct Stack));
  if (!stackfile->stack) {
    die("Memory error");
  }

  if (strcmp(action, "create") == 0) {
    stackfile->file = fopen(filename, "w");
  } else {
    stackfile->file = fopen(filename, "r+");
  }

  return stackfile;
}

void Stackfile_create(struct StackFile *stackfile) {
  stackfile->stack->pos = 0;
}

void Stackfile_read(struct StackFile *stackfile) {
  int rc = fread(stackfile->stack, sizeof(struct Stack), 1, stackfile->file);
  if (rc != 1) {
    die("Failed to load stackfile");
  }
}

void Stackfile_write(struct StackFile *stackfile) {
  rewind(stackfile->file);

  int rc = fwrite(stackfile->stack, sizeof(struct Stack), 1, stackfile->file);
  if (rc != 1) {
    die("Failed to write stackfile");
  }

  rc = fflush(stackfile->file);
  if (rc != 0) {
    die("Cannot flush stackfile");
  }
}

void Stack_push(struct Stack *stack, int push_value) {
  if (stack->pos == STACK_SIZE) {
    die("stack overflow");
  }
  stack->entries[stack->pos] = push_value;
  stack->pos++;
}

void Stack_pop(struct Stack *stack) {
  if (stack->pos == 0) {
    die("stack underflow");
  }
  stack->pos--;
  int pop_value = stack->entries[stack->pos];
  printf("popped %d\n", pop_value);
}

void Stack_is_empty(struct Stack *stack) {
  if (stack->pos == 0) {
    printf("yes\n");
  } else {
    printf("no\n");
  }
}

void Stack_peek(struct Stack *stack) {
  if (stack->pos == 0) {
    printf("null\n");
  } else {
    printf("%d\n", stack->entries[stack->pos - 1]);
  }
}

void Stack_print(struct Stack *stack) {
  for (int i = 0; i < stack->pos; i++) {
    printf("%d ", stack->entries[i]);
  }
  printf("\n");
}

int main(int argc, char **argv) {
  if (argc < 3) {
    die("USAGE: stack <filename> <action> [action params]");
  }

  char *filename = argv[1];
  char *action = argv[2];
  struct StackFile *stackfile = Stackfile_open(filename, action);

  if (strcmp(action, "create") == 0) {
    Stackfile_create(stackfile);
    Stackfile_write(stackfile);
  } else if (strcmp(action, "push") == 0) {
    Stackfile_read(stackfile);
    int pushValue = atoi(argv[3]);
    Stack_push(stackfile->stack, pushValue);
    Stackfile_write(stackfile);
  } else if (strcmp(action, "pop") == 0) {
    Stackfile_read(stackfile);
    Stack_pop(stackfile->stack);
    Stackfile_write(stackfile);
  } else if (strcmp(action, "peek") == 0) {
    Stackfile_read(stackfile);
    Stack_peek(stackfile->stack);
  } else if (strcmp(action, "isempty") == 0) {
    Stackfile_read(stackfile);
    Stack_is_empty(stackfile->stack);
  } else if (strcmp(action, "print") == 0) {
    Stackfile_read(stackfile);
    Stack_print(stackfile->stack);
  } else {
    die("ACTIONS: create push pop peek isempty print");
  }

  return 0;
}
