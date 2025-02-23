#include <stdio.h>
void printint(long x) {
  printf("%ld\n", x);
}
int fred(){
  return (20);
}

void main(){
  int result;
  printint(10);
  result = fred(15);
  printint(result);
  printint(fred(15)+10);
  
}
