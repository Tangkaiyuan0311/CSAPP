#include<stdio.h>
#include<malloc.h> //local or global header file

int main(){
  int* p = malloc(32);
  free(p);
  return 0;
}
