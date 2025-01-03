#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of the declarations


// A variable is declared like int a; Therefore we have to make sure that there is a int followed by a indentifier and a semicolon.
void var_declaration(void){
   match(T_INT,"int");//match will match the current keyword with int and simulatneously find the next token or identifier
   ident();
   addglob(Text);
   genglobsym(Text);
   semi();
}
