#include "defs.h"
#include "data.h"
#include "decl.h"

//Parse the primary factor and return the AST node representing it.
static struct ASTnode *primary(void){
  struct ASTnode *n;

  switch(Token.token){
    case T_INTLIT:
	 n = mkastleaf(A_INTLIT,Token.intvalue);
         scan(&Token);
	 return n;
    default:
	 fprintf(stderr,"syntax error on line %d, token %d\n", Line,Token.token);
	 exit(1);
  }
}

int arithop(int tokentype){
  switch(tokentype){
   case T_PLUS:
	return A_ADD;
   case T_MINUS:
	return A_SUBTRACT;
   case T_STAR:
	return A_MULTIPLY;
   case T_SLASH:
	return A_DIVIDE;
   default: 
	fprintf(stderr,"syntax on line %d, token %d\n",Line,tokentype);
	exit(1);
  
  }
}
//                            EOF  +  -   *  /  INTLIT
static int OpPrec[] = {0,   10,10,20,20,  0};

//function to return the operator precendce of a token
static int op_precedence(int tokentype){
  int prec = OpPrec[tokentype];
  if(prec == 0){
    //a error might have occured
    fprintf(stderr,"A syntax error on line %d, token %d",Line,tokentype);
    exit(1); 
  } 
  return prec;
}

//We will return a AST tree whose root is binary operator.
//The ptr is the precedence of the previous token.
struct ASTnode *binexpr(int ptr){
  struct ASTnode *left,*right;
  int tokentype;

  left = primary();//get the integer literal on the left and also find the next token which will be a operator.
  
  //If no token left return the left pointer.
  tokentype = Token.token;
  if(tokentype == T_EOF){
    return left;
  }
  
  //While the precedence of this token is greater than that of prev token we keep looping.
  while(op_precedence(tokentype)>ptr){
      
    //scan the next integer token
    scan(&Token);
    
    //recursively call binary expression with the precedence of our token to build our subtree.
    right = binexpr(OpPrec[tokentype]);


    //Join the subtree with ours.
    left = mkastnode(arithop(tokentype),left,right,0);

    //update the details of the current token. If no token left,return just the left node.
    tokentype = Token.token;
    if(tokentype == T_EOF){
      return left;
    }
  }
  return left;
}
