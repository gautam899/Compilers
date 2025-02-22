#include "defs.h"
#include "data.h"
#include "decl.h"

// All the miscellaneous functions lie in this .c file
void match(int t,char *what){
  if(Token.token == t){
    //Subsequenty scan the next token
    scan(&Token);
  }
  else{
    fatals("Expected",what);
  }
}

//Match the semicolon
void semi(void){
  match(T_SEMI,";");
}

//Match the left brace
void lbrace(void){
  match(T_LBRACE,"{");
}

//Match the right brace
void rbrace(void){
  match(T_RBRACE,"}");
}

//Match the left parenthesis
void lparen(void){
  match(T_LPAREN,"(");
}

//Match the right parentesis
void rparen(void){
  match(T_RPAREN,")");
}

void ident(void){
  match(T_IDENT,"identifier");
}

// Print out fatal messages
void fatal(char *s) {
  fprintf(stderr, "%s on line %d\n", s, Line);
  exit(1);
}

void fatals(char *s1, char *s2) {
  fprintf(stderr, "%s:%s on line %d\n", s1, s2, Line);
  exit(1);
}

void fatald(char *s, int d) {
  fprintf(stderr, "%s:%d on line %d\n", s, d, Line);
  exit(1);
}

void fatalc(char *s, int c) {
  fprintf(stderr, "%s:%c on line %d\n", s, c, Line);
  exit(1);
}
