#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
//the token types
enum{
  T_EOF, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT
};

//the structure of the token

struct token{
	int token;
	int intvalue;
};

//the ast node types
enum {
  A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT
};

//The abstract syntax tree node

struct ASTnode{
  int op;
  struct ASTnode *left;
  struct ASTnode *right;
  int intvalue;
};

