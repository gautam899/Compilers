#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of statements
// Copyright (c) 2019 Warren Toomey, GPL3

// compound_statement:          // empty, i.e. no statement
//      |      statement
//      |      statement statements
//      ;
//
// statement: print_statement
//      |     declaration
//      |     assignment_statement
//      |     if_statement
//      |     while_statement
//      ;

// print_statement: 'print' expression ';'  ;
//

static struct ASTnode *single_statement(void);

static struct ASTnode *print_statement(void) {
  struct ASTnode *tree;
  int reg;

  // Match a 'print' as the first token
  match(T_PRINT, "print");

  // Parse the following expression
  tree = binexpr(0);

  // Make an print AST tree
  tree = mkastunary(A_PRINT, tree, 0);

  // The compound statement will take care of the matching of the semicolon.
  // return the AST
  return (tree);
}

// assignment_statement: identifier '=' expression ';'   ;
//
static struct ASTnode *assignment_statement(void) {
  struct ASTnode *left, *right, *tree;
  int id;

  // Ensure we have an identifier
  ident();

  // Check it's been defined then make a leaf node for it
  if ((id = findglob(Text)) == -1) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, id);

  // Ensure we have an equals sign
  match(T_ASSIGN, "=");

  // Parse the following expression
  left = binexpr(0);

  // Make an assignment AST tree
  tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

  // We will not match the semicolon in the the assigment. The semicolon is matched only if the assignment statement is called from the compound statement.
  // Return the AST.
  return (tree);
}

// if_statement: if_head
//      |        if_head 'else' compound_statement
//      ;
//
// if_head: 'if' '(' true_false_expression ')' compound_statement  ;
//
// Parse an IF statement including
// any optional ELSE clause
// and return its AST
struct ASTnode *if_statement(void) {
  struct ASTnode *condAST, *trueAST, *falseAST = NULL;

  // Ensure we have 'if' '('
  match(T_IF, "if");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  trueAST = compound_statement();

  // If we have an 'else', skip it
  // and get the AST for the compound statement
  if (Token.token == T_ELSE) {
    scan(&Token);
    falseAST = compound_statement();
  }
  // Build and return the AST for this statement
  return (mkastnode(A_IF, condAST, trueAST, falseAST, 0));
}


// while_statement: 'while' '(' true_false_expression ')' compound_statement  ;
//
// Parse a WHILE statement
// and return its AST

struct ASTnode *do_while_statement(void){

  struct ASTnode *cond_AST,*body_AST;
  //we match the do first
  match(T_DO,"do");
  

  //Evaluate the body
  body_AST = compound_statement();
  
  //The right brace is matched in the compound statement only. So just match the while keyword and form the conditonal AST using the conditoon given.
  match(T_WHILE,"while");
  //Match the opening parenthesis
  lparen();
  cond_AST = binexpr(0);
  if(cond_AST->op<A_EQ || cond_AST->op > A_GE){
    fatal("Bad comparison operator"); 
  }
  //match the right parenthesis
  rparen();
  //Match the semi;
  semi();
  return mkastnode(A_DO,cond_AST,NULL,body_AST,0);

}
struct ASTnode *while_statement(void) {
  struct ASTnode *condAST, *bodyAST;

  // Ensure we have 'while' '('
  match(T_WHILE, "while");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  bodyAST = compound_statement();

  // Build and return the AST for this statement
  return (mkastnode(A_WHILE, condAST, NULL, bodyAST, 0));
}

//Parse for the 'for' statement and return its AST.
static struct ASTnode* for_statement(void){
   struct ASTnode *condAST,*bodyAST;
   struct ASTnode *preopAST, *postopAST;
  struct ASTnode *tree;

  // Ensure we have 'for' '('
  match(T_FOR, "for");
  lparen();

  // Get the pre_op statement and the ';'
  preopAST = single_statement();
  semi();

  // Get the condition and the ';'
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  semi();

  // Get the post_op statement and the ')'
  postopAST = single_statement();
  rparen();

  // Get the compound statement which is the body
  bodyAST = compound_statement();

  // For now, all four sub-trees have to be non-NULL.
  // Later on, we'll change the semantics for when some are missing

  // Glue the compound statement and the postop tree
  tree = mkastnode(A_GLUE, bodyAST, NULL, postopAST, 0);

  // Make a WHILE loop with the condition and this new body
  tree = mkastnode(A_WHILE, condAST, NULL, tree, 0);

  // And glue the preop tree to the A_WHILE tree
  return (mkastnode(A_GLUE, preopAST, NULL, tree, 0));
}

static struct ASTnode *single_statement(void){
  switch (Token.token) {
      case T_PRINT:
	return print_statement();
      case T_INT:
	var_declaration();
	return NULL;		// No AST generated here
      case T_IDENT:
	return assignment_statement();
      case T_IF:
	return if_statement();
      case T_WHILE:
	return while_statement();
      case T_DO:
	return do_while_statement(); 
      case T_FOR:
        return for_statement();	
      default:
	fatald("Syntax error, token", Token.token);
    }

}
// Parse a compound statement
// and return its AST
struct ASTnode *compound_statement(void) {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  // Require a left curly bracket
  lbrace();

  while (1) {
    // Some statements must be follwed by a semicolon.
    tree = single_statement();
    if(tree != NULL && (tree->op == A_PRINT || tree->op == A_ASSIGN)){
      semi();
    }
    // For each new tree, either save it in left
    // if left is empty, or glue the left and the
    // new tree together
    if (tree) {
      if (left == NULL)
	left = tree;
      else
	left = mkastnode(A_GLUE, left, NULL, tree, 0);
    }
    // When we hit a right curly bracket, skip past and return the the AST.
    if(Token.token == T_RBRACE){
       rbrace();
       return left;
    }

  }
}
