#include "defs.h"
#include "data.h"
#include "decl.h"


//The miscllaneous functions like match and etc.

void match(int t,char *what){
   if(Token.token == t){
     //If matched then find the next token simulataneously
     scan(&Token);
   }
   else{
     fatals("Expected",what);
   }
}

// Match a semicolon and fetch the next token
void semi(void){
  match(T_SEMI,";");
}

// Match an identifier and fetch the next token
void ident(void){
  match(T_IDENT,"identifier");
}

void fatal(char *s){
  fprintf(stderr,"%s on line %d\n",s,Line);
  exit(1);
}

void fatals(char *s1,char *s2){
  fprintf(stderr,"%s:%s on line %d\n",s1,s2,Line);
  exit(1);
}

void fatald(char *s,int d){
  fprintf(stderr,"%s:%d on line %d\n",s,d,Line);
  exit(1);
}

void fatalc(char *s,int c){
  fprintf(stderr,"%s:%d on line %d\n",s,c,Line);
  exit(1);
}

