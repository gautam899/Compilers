#include "defs.h"
#include "data.h"
#include "decl.h"

static int charpos(char *s,int c){
  char *p;
  p = strchr(s,c);
  return p ? p-s:-1;
}
static int next(void){
  int c;

  if(Putback){
    c = Putback;
    Putback = 0;
    return c;
  }
  c = fgetc(Infile);
  if('\n' == c){
    Line++;
  }
  return c;

}
void putback(int c){
  Putback = c;
}
static int scanint(int c){
  int k = 0,val=0;

  while((k = charpos("0123456789",c)) >= 0){
    val = val*10 + k;
    c = next();
  }
  putback(c);
  return val;
}

static int skip(void){
  int c;
  c = next();

  while(' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
    //keep finding the next
    c = next();
  }
  return c;
 
}

int scan(struct token *t){
  int c;
  c = skip();
  switch(c){
     case EOF:
	  t->token = T_EOF;
	  return 0;
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
    // literal integer value in
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



