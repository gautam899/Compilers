#include "defs.h"
#include "data.h"
#include "decl.h"

static int charpos(char *s,int c){
  //c pointes to the first index of the string.
  char *p;
  p = strchr(s,c);//returns a pointer to the first occurence of the character c(the ascii value).
  return (p ? p-s : -1);//we return the difference between the address values.  
}

static int next(void){
  int c;
  //If putback is holding a character then we return the putback character.
  if(Putback){
    c = Putback;
    Putback = 0;
    return c;
  }

  c = fgetc(Infile);

  if(c == '\n'){
    Line++;
  }
  return c;
}

static void putback(int c){
  Putback = c;
}

//Skip all the unwanted character
static int skip(){
  int c;
  c = next();

  while(' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
    c = next();
  }
  return c;
}

static int scanint(int c){
  int k,val=0;

  while((k=charpos("0123456789",c))>=0){
     val = val*10 + k;
     c=next();
  }
  //If we reach a non-integer literal we put it back into the stream as we will need it later.
  putback(c);
  return val;

}

//the usual scanning part
int scan(struct token *t){
  int c;
  //skip the non-usefull character
  c = skip();

  // Determine the token 
   // Determine the token based on
  // the input character
  switch (c) {
  case EOF:
    t->token = T_EOF;
    return (0);
  case '+':
    t->token = T_PLUS;
    break;
  case '-':
    t->token = T_MINUS;
    break;
  case '*':
    t->token = T_STAR;
    break;
  case '/':
    t->token = T_SLASH;
    break;
  default:

    // If it's a digit, scan the
    // literal integer value in and find the value. This means that we are actually sending the ascii value of the int and finding out using the scanint function what exactly that interger value is.
    if (isdigit(c)) {
      t->intvalue = scanint(c);
      t->token = T_INTLIT;
      break;
    }

    printf("Unrecognised character %c on line %d\n", c, Line);
    exit(1);
  }

  // We found a token
  return (1);

}
