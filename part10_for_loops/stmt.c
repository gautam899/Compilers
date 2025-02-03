#include "defs.h"
#include "data.h"
#include "decl.h"

//In here we will parse all the statements.

static struct ASTnode *single_statement(void);

static struct ASTnode *print_statement(void){
  struct ASTnode *tree;
  int reg;

  //Match the keyword print
  match(T_PRINT,"print");
  
  //Evaluate the binary expression
  tree = binexpr(0);
  
  // Make an print AST tree
  tree = mkastunary(A_PRINT,tree,0);
  
  // Return the AST
  return tree;
  
}//Note before returning the tree in print statement we intially we matching semicolon but now we have removed it. We will let compound statement take care of it.

static struct ASTnode *assignment_statement(void){
  struct ASTnode *left,*right,*tree;
  int id;
  int reg;
  //First we have to make sure we have an identifier.
  ident();
  
  //Check if its a valid identifier.
  if((id = findglob(Text)) == -1){
    fatals("Undeclared variable",Text);
  }

  right = mkastleaf(A_LVIDENT,id);

  //Match the assignment operator(=)
  match(T_ASSIGN,"=");

  //Parse the expression following the assignment operator.
  left = binexpr(0);

  //make a assignment tree.
  tree = mkastnode(A_ASSIGN,left,NULL,right,0);
  return tree;//In here also we have removed the matching of semicolon unlike what we were doing earlier.

}

// The if statement
static struct ASTnode *if_statement(void){
  struct ASTnode *condAST,*trueAST,*falseAST=NULL;
  // Match the if keyword
  match(T_IF,"if");
  lparen();
  // Now evaluate the following expression 
  condAST = binexpr(0);
  //Before moving ahead we need to make sure that if the operator is a valid operator.
  if(condAST->op < A_EQ || condAST->op > A_GE){
    fatal("Bad comparision");
  }
  //match the closing parenthsis
  rparen();

  trueAST = compound_statement();
  //If the token is else we skip and get the AST for the compound statemnet.
  if(Token.token == T_ELSE){
    scan(&Token);
    falseAST = compound_statement();
  }

  return mkastnode(A_IF,condAST,trueAST,falseAST,0);
}

static struct ASTnode *while_statement(void){
  struct ASTnode *condAST, *trueAST;

  //match the while keyword
  match(T_WHILE,"while");

  lparen();
  condAST = binexpr(0);
  //Make sure we have valid comparision operator
  if(condAST->op<A_EQ || condAST->op > A_GE){
    fatal("Bad operator");
  }
  rparen();

  trueAST = compound_statement();
  return mkastnode(A_WHILE,condAST,NULL,trueAST,0);
}

//Now comes the for statement
// for_statement: 'for' '(' preop_statement ; true_false_expression; post_op  ')' compound_statement
//We will parse the for statement and return its AST.
static struct ASTnode *for_statement(void){
  struct ASTnode *condAST,*bodyAST;
  struct ASTnode *preopAST,*postopAST;
  struct ASTnode *tree;

  //Match the for keyword
  match(T_FOR,"for");
  //Match the opening parenthesis
  lparen();

  //Match the preop statement and a semicolon
  preopAST = single_statement();
  semi();
  //Evalutate the conditional AST and also match the semicolon.
  condAST = binexpr(0);
  if(condAST->op < A_EQ || condAST->op > A_GE){
    fatal("Bad comparision");
  }
  semi();

  //Now the post_op statement;
  //We are parsing the postop statement before the compound st. But when we generate code i.e the assembly one the code for the postop is generated after the compound statement.
  postopAST = single_statement();
  rparen();
  
  //Now parse the compound statement
  bodyAST = compound_statement();
  
  //Here we are mandating that all the four subtrees have to be non-null. 
  tree = mkastnode(A_GLUE, bodyAST, NULL,postopAST,0);//The reason why we are glueing the bodyAST and the postOp together is that if the condition fails then we wish to skip the body and the postop all together.

  tree = mkastnode(A_WHILE, condAST, NULL,tree,0);

  tree = mkastnode(A_GLUE, preopAST,NULL,tree,0);

  return tree;
  //If we now iterate the above formed 
}

//Parse a single statement and return its AST
static struct ASTnode *single_statement(void){
   switch (Token.token) {
    case T_PRINT:
      return (print_statement());
    case T_INT:
      var_declaration();
      return (NULL);		// No AST generated here
    case T_IDENT:
      return (assignment_statement());
    case T_IF:
      return (if_statement());
    case T_WHILE:
      return (while_statement());
    case T_FOR:
      return (for_statement());
    default:
      fatald("Syntax error, token", Token.token);
  }
}

//Parse the compound statement

struct ASTnode *compound_statement(void){
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  //match the left brace
  lbrace();

  while(1){
    tree = single_statement();

    // Some statement must be followed by a semicolon
    if(tree!=NULL && (tree->op == A_PRINT || tree->op == A_ASSIGN)){
      semi();
    }

    //For each new tree, either we save it in left or we glue the left and tree together
    if(tree != NULL){
      if(left == NULL){
	  left = tree;
      }
      else{
	   left = mkastnode(A_GLUE,left,NULL,tree,0);
      }
    }

    //If we hit the right curly brace
    if(Token.token == T_RBRACE){
      rbrace();
      return left;
    }
  }
}
