#include "defs.h"
#include "data.h"
#include "decl.h"

static struct ASTnode *primary(void){
   struct ASTnode *n;
   int id;
   switch(Token.token){
	   case T_INTLIT:
		   n = mkastleaf(A_INTLIT,Token.intvalue);
		   break;
	   case T_IDENT:
		   //check if the identifier exist or not 
		   id = findglob(Text);
		   if(id == -1){
		     fatals("Unknown Variable",Text);
		   }
		   n = mkastleaf(A_IDENT,id);
		   break;
	  default:
		   fatald("Syntax Error",Token.token);
   }
   scan(&Token);
   return n;

}

//Convert the binary token into AST operation. This will be done using 1:1 matching 
//as will help reducing the size of the code otherwise the number of switch case will keep increasing.
static int arithop(int tokentype){
 
	if(tokentype > T_EOF && tokentype < T_INTLIT){
	  return tokentype;
	}
	fatald("Syntax Error, token",tokentype);
         
}

static int OpPrec[] = {0,10,10,20,20,30,30,40,40,40,40};

//Check if we have a binary operator and return its precedence.
static int op_precedence(int tokentype){
   int prec = OpPrec[tokentype];
   if(prec == 0){
      fatald("Syntax Error, Token",tokentype);
   }
   return prec;
}

struct ASTnode *binexpr(int ptp){
  struct ASTnode *left,*right;
  int tokentype;


  // Get the primary tree on the left
  left = primary();
  tokentype = Token.token;
  //Update the token type
  if(tokentype == T_SEMI){
    return left;
  }
  //Currently token is a operator.
  while(op_precedence(tokentype)>ptp){
    //Scan the next integer literal.
    scan(&Token);
    
    //Now form the right side of the operator.
    right = binexpr(OpPrec[tokentype]);
    
    //Connect the subtree with ours.
    left = mkastnode(arithop(tokentype),left,right,0);
    
    // Update the tokentype.
    tokentype = Token.token;

    if(tokentype == T_SEMI){
      return left;
    }
  }
  // Return the tree if the operator precedence is less or same
  return left;
}
