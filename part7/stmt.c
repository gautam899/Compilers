#include "defs.h"
#include "data.h"
#include "decl.h"

void print_statement(void){
   struct ASTnode *tree;
   int reg;

   // Match a 'print' as the first token
   match(T_PRINT,"print");

   // Parse the following expression and 
   // generate the assembly code
   tree = binexpr(0);
   reg = genAST(tree,-1);
   genprintint(reg);
   genfreeregs();
   
   // Match the following semicolon and find the next token simultaneouly
   semi();
}

void assignment_statement(void){
   struct ASTnode *left,*right,*tree;
   int id;
   //left will contain the expression and the right will contain the identifier.

   //ensure we have identifier and simultaneouly find the next token
   ident();

   if((id = findglob(Text)) == -1){
      //if the identifier do not occur in the global table
      fatals("Undeclared identifier ", Text);
   }

   right = mkastleaf(A_LVIDENT,id);

   //match the equal
   match(T_ASSIGN,"=");
 
   //Evaluate the following expression 
   left = binexpr(0);

   tree = mkastnode(A_ASSIGN,left,right,0);

   //Generate the assemeble code
   genAST(tree,-1);
   genfreeregs();
   //match the semicolon and simultaneusly find the next token
   semi();
}



// Parse one or more statements
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
