#include "defs.h"
#include "data.h"
#include "decl.h"

// Miscellaneous functions

// Ensure that the current token is t,
// and fetch the next token.
void match(int t,char *what){
  if(Token.token == t){
    scan(&Token);
  }
  else{
    printf("%s expected on line %d\n", what, Line);
    exit(1);
  }
}

void semi(void){
  match(T_SEMI,";");
}
