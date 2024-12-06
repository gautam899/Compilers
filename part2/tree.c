//In this file we will define all the extern function which are creating the
//leaf node , or the node with left and right

#include "defs.h"
#include "data.h"
#include "decl.h"

struct ASTnode *mkastnode(int op, struct ASTnode *left,struct ASTnode *right,int intvalue){
  struct ASTnode *n;

  //Create a memory space for the new node using malloc of the c

  n = (struct ASTnode *)malloc(sizeof(struct ASTnode));
  if(n==NULL){
    fprintf(stderr,"Unable to malloc in mkastnode()\n");
    exit(1);
  }

  n->op = op;
  n->left = left;
  n->right = right;
  n->intvalue = intvalue;
  return n;
}

//For leaf node

struct ASTnode *mkastleaf(int op,int intvalue){
  return mkastnode(op,NULL,NULL,intvalue); 
}

//For unary node i.e onl one child node
struct ASTnode *mkastunary(int op,struct ASTnode*left,int intvalue){
  return mkastnode(op,left,NULL,intvalue);
}
