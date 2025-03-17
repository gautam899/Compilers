#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of declarations
// Copyright (c) 2019 Warren Toomey, GPL3



// global_declarations : global_declarations
//      | global_declaration global_declarations
//      ;
//
// global_declaration: function_declaration | var_declaration ;
//
// function_declaration: type identifier '(' ')' compound_statement   ;
//
// var_declaration: type identifier_list ';'  ;
//
// type: type_keyword opt_pointer  ;
//
// type_keyword: 'void' | 'char' | 'int' | 'long'  ;
//
// opt_pointer: <empty> | '*' opt_pointer  ;
//
// identifier_list: identifier | identifier ',' identifier_list ;
//


// Parse the current token and
// return a primitive type enum value. 
// Also scan in the next token.
int parse_type(void) {
  int type;
  switch (Token.token) {
    case T_VOID:
      type = P_VOID;
      break;
    case T_CHAR:
      type = P_CHAR;
      break;
    case T_INT:
      type = P_INT;
      break;
    case T_LONG:
      type = P_LONG;
      break;
    default:
      fatald("Illegal type, token", Token.token);
  }

  // Scan in one or more further '*' tokens 
  // and determine the correct pointer type
  while (1) {
    scan(&Token);
    if (Token.token != T_STAR)
      break;
    type = pointer_to(type);
  }

  // We leave with the next token already scanned
  return (type);
}

// Parse the declaration of a variable
// variable_declaration: type identifier ';';
// Parse the declaration of list of variables.
// In this part var_declaration need a pre-calculated variable and its type.
void var_declaration(int type) {
  int id;
  
  while(1){
    // Text now has the identifier's name. Add it as a known identifier.
    // Add it as known idenifier.
    // and generate its space in assembly.
    id = addglob(Text,type,S_VARIABLE,0);
    genglobsym(id);

    //Now there can be two things happening here. Either the next token can be a semicolon or a comma.
    if(Token.token == T_SEMI){
      scan(&Token);
      return;
    }

    //If the next token is a comma then skip the comma, calculate the next token and loop back
    if(Token.token == T_COMMA){
      scan(&Token);
      ident();
      continue;
    }
    
    // If neither a comma nor a semicolon then something is missing.
    fatal("Missing , or ; after the identifier");
  }
}

// This time when the function_declaration gets called from global_declation we already have the identifier and its type scanned
struct ASTnode *function_declaration(int type){
  struct ASTnode *tree,*finalstmt;
  int nameslot, endlabel;

  // Text now has the identifier's name
  // Get a label-id for the end label, add the function to the symbol table, and set the functionid global to the function's symbol-id
  endlabel = genlabel();
  nameslot = addglob(Text,type,S_FUNCTION, endlabel);
  Functionid = nameslot; //Function id for a function is set every time there is function declaration.
  //Scan the parenthesis
  lparen();
  rparen();

  // Get the ast tree for compound statement
  tree = compound_statement();

  // If the function type is not void then check that the last AST operationin the compound statement was a return statement
  if(type != P_VOID)
  { 
    // Error if no statements in the function
    if (tree == NULL)
      fatal("No statements in function with non-void type");
    finalstmt = (tree->op == A_GLUE) ? tree->right : tree;
    if(finalstmt == NULL || finalstmt->op != A_RETURN){
      fatal("No return for function with non-void type");
    }
  }
  
  //Return the A_FUNCTION node that has the function nameslot and the compound statement subtree.
  return mkastunary(A_FUNCTION,P_VOID,tree,nameslot);

}

// Parse one or more global declarations, either variables or functions.
void global_declarations(void){
  struct ASTnode *tree;
  int type;

  while(1){
    // We need to read past the type and identifier to check if its a variable declaration or a function declaration. 
    type = parse_type();//This will parse the type and calcualte the next token as well. 
    ident(); //This will calculate one further token next to the identifier.

    if(Token.token == T_LPAREN){
      //Parse the function declaration and generate the assembly code for it.
      tree = function_declaration(type);
      genAST(tree, NOREG, 0);
    }else{
      var_declaration(type);
    }

    if(Token.token == T_EOF){
      break;
    }
  }
}
