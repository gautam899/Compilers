#include "defs.h"
#include "data.h"
#include "decl.h"

// variable_declaration 'int' identifier ';'
// Parse the declaration of a variable.

void var_declaration(void){
  match(T_INT,"int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}

struct ASTnode *function_declaration(void){
  struct ASTnode *tree;
  int nameslot;

  //Match the void keyword
  match(T_VOID,"void");
  //Match the following identifier. We are treating the function name here as identifier.
  ident();
  nameslot = addglob(Text);
  lparen();
  rparen();

  //get the AST tree for the compound statement.
  tree = compound_statement();
  //We will return an A_FUNCTION node which has the function's nameslot 
  // and the compound statement sub-tree.
  return mkastunary(A_FUNCTION,tree,nameslot);

}
