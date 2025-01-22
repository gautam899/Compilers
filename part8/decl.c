#include "defs.h"
#include "data.h"
#include "decl.h"

// Parse and declare the variable.
void var_declaration(void){
  // We need to ensure that we have a int followed by a identifier followed by a semicolon
  match(T_INT,"int");
  ident();//match the identifier
  addglob(Text);//Add the global symbol to the symbol table.
  genglobsym(Text);//
  semi();//match the semi colon
}
