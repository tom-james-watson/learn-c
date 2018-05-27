#include <stdio.h>

void myfunc() {

  printf("From myfunc\n");

}

int main() {

  int age = 10;
  int height =  72;

  printf("I am %d years old.\n", age);
  printf("I am %d inches tall.\n", height);

  printf("Color %s, number1 %d, number2 % 5d, hex %#x, float %.3f, unsigned value %u.\n", "red", 123456, 89, 255, 3.14159, 250);

  myfunc();

  return 0;

}
