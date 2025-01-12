#include "defs.h"
#include "data.h"
#include "decl.h"

int operatorType;
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

// We are assuming we only get comparision operator
// 1. We already have the first value. We will try to make whole integer out of it. There might be integer with more that one digit.
// 2. Then comes the operator. 
// 3. Then comes again the first step of evalutating the integer
// 4. Then we can make a node of the final value.
struct ASTnode *binexpr(int ptp){
  struct ASTnode *node;
  int tokentype,ans=0;
  //At this stage the token will be 
  //Token->token = 16(intval) and Token->intvalue = (some value)
  
  int val1 = Token.intvalue;

  //Scan the operator.
  scan(&Token);

  //Update the token type
  tokentype = Token.token;

  //find the value on the right side of the operator.
  scan(&Token);
  int val2 = Token.intvalue;
  
  switch(tokentype){
        case T_EQ:
		  if(val1==val2){
		    ans = 1;
		  }
		  break;
        case T_NE:
		  if(val1!=val2){
		    ans = 1;
		  }
		  break;
        case T_LT:
		  if(val1<val2){
		    ans = 1;
		  }
		  break;	 		  
        case T_GT:
		  if(val1>val2){
		    ans = 1;
		  }
		  break;
       	case T_LE:
		  if(val1<=val2){
		    ans = 1;
		  }
		  break;
        case T_GE:
		  if(val1>=val2){
		    ans = 1;
		  }
		  break;
        default: 
		  fatalc("Unrecognized comparision operator.",tokentype);
  }

  //we have the answer now and now we can create the node
  node = mkastleaf(A_INTLIT,ans);

  //Next token will be a semicolon for sure if the expression is right
  scan(&Token);

  tokentype = Token.token;

  //Update the token type
  if(tokentype == T_SEMI){
    return node;
  }
  return node;
}
