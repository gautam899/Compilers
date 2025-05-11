char* str;
int a;
int main(){
  a = 10;
  printint(10);
  for(str = "Hello\n"; *str != 0; str++){
     printchar(*str);
  }
  return (0);
}
