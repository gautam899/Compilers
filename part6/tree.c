#include "defs.h"
#include "data.h"
#include "decl.h"

// Build and return a generic AST node
struct ASTnode *mkastnode(int op,struct ASTnode *left,struct ASTnode *right,int intvalue){
  struct ASTnode *n;

  n = (struct ASTnode *)malloc(sizeof(struct ASTnode));
  
  if(n == NULL){
    fatal("Unable to malloc in mkastnode()");
  }
  
  n->op = op;
  n->left = left;
  n->right = right;
  n->v.intvalue = intvalue;
  return n;
}

//Leaf node
struct ASTnode *mkastleaf(int op,int intvalue){
  return mkastnode(op,NULL,NULL,intvalue);
}

//Make a unary node: only left child
struct ASTnode *mkastunary(int op,struct ASTnode *left, int intvalue){
  return mkastnode(op,left,NULL,intvalue);
}
