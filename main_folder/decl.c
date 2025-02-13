#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of declarations
// Copyright (c) 2019 Warren Toomey, GPL3


// declaration: 'int' identifier ';'  ;
//
// Parse the declaration of a variable
void var_declaration(void) {

  // Ensure we have an 'int' token followed by an identifier
  // and a semicolon. Text now has the identifier's name.
  // Add it as a known identifier
  match(T_INT, "int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}

struct ASTnode *function_declaration(void){
  struct ASTnode *tree;
  int nameslot;

  //Find the void, match the identifier or the function name and the empty circular parenthesis. 
  match(T_VOID,"void");
  ident();
  nameslot = addglob(Text);
  lparen();
  rparen();
  tree = compound_statement();

  //Return the A_FUNCTION node that has the function nameslot and the compound statement subtree.
  return mkastunary(A_FUNCTION,tree,nameslot);

}
