#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of expressions
// Copyright (c) 2019 Warren Toomey, GPL3

// Parse a primary factor and return an
// AST node representing it.
static struct ASTnode *primary(void) {
  struct ASTnode *n;

  // For an INTLIT token, make a leaf AST node for it
  // and scan in the next token. Otherwise, a syntax error
  // for any other token type.
  switch (Token.token) {
    case T_INTLIT:
      n = mkastleaf(A_INTLIT, Token.intvalue);
      scan(&Token);
      return (n);
    default:
      fprintf(stderr, "syntax error on line %d, token %d\n", Line,
	      Token.token);
      exit(1);
  }
}

// Convert a binary operator token into an AST operation.
static int arithop(int tokentype) {
  switch (tokentype) {
    case T_PLUS:
      return (A_ADD);
    case T_MINUS:
      return (A_SUBTRACT);
    case T_STAR:
      return (A_MULTIPLY);
    case T_SLASH:
      return (A_DIVIDE);
    default:
      fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
      exit(1);
  }
}

// Operator precedence for each token
static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };

// Check that we have a binary operator and
// return its precedence.
static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (prec == 0) {
    fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
    exit(1);
  }
  return (prec);
}

//ptp is the precedence of the prev pointer
struct ASTnode *binexpr(int ptp){
   struct ASTnode *left,*right;
   int tokentype;

   left = primary();

   tokentype = Token.token;
   if(tokentype == T_SEMI){
     return left;
   }

   while(op_precedence(tokentype) > ptp){
   	
	scan(&Token);

	right = binexpr(OpPrec[tokentype]);

	left = mkastnode(arithop(tokentype),left,right,0);
	
	//While calling the function recurcively the token would have been updated so we need to update the tokentype corresponding to the token before moving to the next tokentype.
	tokentype = Token.token;
	if(tokentype == T_SEMI){
	  return left;
	}
     
   }
   //return the tree if the precedence we have is less or same
   return left;
}
