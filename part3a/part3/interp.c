#include "defs.h"
#include "data.h"
#include "decl.h"

//list of ast operators.

static char *ASTop[] = {"+","-","*","/"};

//interpret the tree recurcively

int interpretAST(struct ASTnode *n){
  int leftval,rightval;

  if(n->left){
     leftval = interpretAST(n->left);
  }
  if(n->right){
     rightval = interpretAST(n->right);
  }

  switch(n->op){
     case A_ADD:
	  return (leftval+rightval);
     case A_SUBTRACT:
	  return (leftval-rightval);
     case A_MULTIPLY:
	  return (leftval*rightval);
     case A_DIVIDE:
	  return (leftval/rightval);
     case A_INTLIT:
	  return n->intvalue;
     default: 
	  fprintf(stderr,"Unkown AST operator %d/n",n->op);
	  exit(1);
  }
}
