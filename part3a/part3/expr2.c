#include "defs.h"
#include "data.h"
#include "decl.h"


//parse the prime factor and parse the next token as well(a operator) and return the node.

static struct ASTnode* primary(void){
  struct ASTnode *n;

  switch(Token.token){
     case T_INTLIT:
	  n = mkastleaf(A_INTLIT,Token.intvalue);
	  scan(&Token);
	  return n;
     default: 
	  fprintf(stderr,"syntax error on line no. %d, token %d\n",Line,Token.token);
	  exit(1);
  }
}

static int arithop(int tok){
  switch(tok){
    case T_PLUS: 
	return A_ADD;
    case T_MINUS:
        return (A_SUBTRACT);
    case T_STAR:
    	return (A_MULTIPLY);
    case T_SLASH:
    	return (A_DIVIDE);
    default:
        fprintf(stderr, "syntax error on line %d, token %d\n", Line, tok);
        exit(1);
   }
}

struct ASTnode* additive_expr(void);

//Return a substree whose root is a '*' or '/' binary operator
struct ASTnode *multiplicative_expr(void){
   struct ASTnode *left,*right;
   int tokentype;
   left = primary();


   tokentype = Token.token;

   if(tokentype == T_EOF){
      return left;
   }
   // Keep f
   while((tokentype == T_STAR)  || (tokentype == T_SLASH)){
      scan(&Token);
      right = primary();


      // Join with left integer literal.
      left = mkastnode(arithop(tokentype),left,right,0);

      // Update the details of the current token(operator)
      tokentype = Token.token;
      
      // If we reach the end of the file we return the left.
      if(tokentype==T_EOF){
        break;
      }
   }
   return left;
}

//Return the AST tree whose root is a '+' or '-' binary operator.
struct ASTnode *additive_expr(void){
   struct ASTnode *left,*right;
   int tokentype;

   // Get the left subtree at a heigher precedence than us. 
   left = multiplicative_expr();
    
   tokentype = Token.token;
   // If tokens are not left then we return the left node.
   if(tokentype == T_EOF){
     return left;
   }

   //Cache the '+' or '-' token type
   while(1){
     //Fetch the next integer literal.
     scan(&Token);
     
     // Get the right sub-tree at a heigher precedence.
     right = multiplicative_expr();

     //Join the two subtree with out low-precedence operator.
     left = mkastnode(arithop(tokentype), left,right,0);

     //And get the new token at our low-precedence operator.
     tokentype = Token.token;
     if(tokentype == T_EOF){
       break;
     }

   }
   return left;
}

struct ASTnode *binexpr(int n){
 return (additive_expr());
}



