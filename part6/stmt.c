#include "defs.h"
#include "data.h"
#include "decl.h"

// statements: statement 
//          | statement statements
//          ;
//
// statement: 'print' expression ';'
//          | 'int' identifier ';'
//          | identifier '=' expression ';'
//          ;
//
// identifier: T_IDENT
//          ;

void print_statement(void){
  struct ASTnode *tree;
  int reg;
  
  //Match print as the first token and simultaneously find the next token
  match(T_PRINT,"print");

  tree = binexpr(0);
  reg = genAST(tree,-1);
  genprintint(reg);
  genfreeregs();

  //Match the following semicolon
  semi();

}

void assignment_statement(void){
  struct ASTnode *left,*right,*tree;
  int id;

  // Ensure we have an identifier
  ident();

  // Check it's been defined then make a leaf node for it.
  if((id = findglob(Text)) == -1){
    fatals("Undeclared variable",Text);
  }

  right = mkastleaf(A_LVIDENT,id);

  // Ensure we have equal size. Match will find the next token simultaneusly
  match(T_EQUALS,"=");
  
  // Parse the following expression
  left = binexpr(0);

  //Make an assgnemnt tree
  tree = mkastnode(A_ASSIGN,left,right,0);

  //Generate the assembly code for the assignement
  genAST(tree, -1);
  genfreeregs();

  //Match the following semicolon.
  semi();


}
//Parse on or more statements
void statements(void){
  while(1){
    switch(Token.token){
      case T_PRINT:
	      print_statement();
	      break;
      case T_INT:
	      var_declaration();
	      break;
      case T_IDENT:
	      assignment_statement();
	      break;
      case T_EOF:
	      return;
      default:
	      fatald("Syntax error, token",Token.token);	        
    
    }
  
  }
}

