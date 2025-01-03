#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of statements
// statements: statement
//           | statement statements
//           ;
// statements: 'print' expression ';'
//           ;

void statements(void){
  struct ASTnode *tree;
  int reg;

  //parse one or more statements
  while(1){
    // Match the first token found in main to be print.
    // Match will also correspondingly find the next token
    match(T_PRINT,"print");

    tree = binexpr(0);
    reg = genAST(tree);
    genprintint(reg);
    genfreeregs();

    // Match the following semicolon
    // and stop if we are at EOF
    semi();
    if(Token.token == T_EOF){
      return;
    }
  }
}
