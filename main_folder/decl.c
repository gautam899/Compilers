#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of declarations
// Copyright (c) 2019 Warren Toomey, GPL3

// Parse the current token and
// return a primitive type enum value
int parse_type(int t) {
  if (t == T_CHAR)
    return (P_CHAR);
  if (t == T_INT)
    return (P_INT);
  if (t == T_VOID)
    return (P_VOID);
  fatald("Illegal type, token", t);
}

// declaration: 'int' identifier ';'  ;
//
// Parse the declaration of a variable
void var_declaration(void) {
  int id,type;
  //Get the type of the variable and then the identifier
  type = parse_type(Token.token);
  scan(&Token);
  ident();
  // Text now has the identifier's name.
  // Add it as a known identifier
  // and generate its space in assembly
  id = addglob(Text, type, S_VARIABLE);
  genglobsym(id);
  // Get the trailing semicolon
  semi();
  
}

struct ASTnode *function_declaration(void){
  struct ASTnode *tree;
  int nameslot;

  //Find the void, match the identifier or the function name and the empty circular parenthesis. 
  match(T_VOID,"void");
  ident();
  nameslot = addglob(Text,P_VOID,S_FUNCTION);
  lparen();
  rparen();
  tree = compound_statement();

  //Return the A_FUNCTION node that has the function nameslot and the compound statement subtree.
  return mkastunary(A_FUNCTION,P_VOID,tree,nameslot);

}
