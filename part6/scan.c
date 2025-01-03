#include "defs.h"
#include "data.h"
#include "decl.h"

// Find the integer corresponding to the ascii value of character
static int chrpos(char *s,int c){
  char *p;
  p = strchr(s,c);
  return p ? p-s:-1;
}

// Next
static int next(void){
  int c=0;

  if(Putback){
    c = Putback;
    Putback=0;
    return c;  
  }

  c = fgetc(Infile);
  if('\n'==c){
    Line++; //If we encounter a new line character then increment the line count.
  }
  return c;
}

static void putback(int c){
   Putback = c;
}

static int skip(){
  int c;
  c = next();
  while(' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c ){
     c = next();
  }
  return c;
}

static int scanint(int c) {
  int k, val = 0;

  // Convert each character into an int value
  while ((k = chrpos("0123456789", c)) >= 0) {
    val = val * 10 + k;
    c = next();
  }

  // We hit a non-integer character, put it back.
  putback(c);
  return (val);
}

static int scanident(int c,char *buf, int lim){
  int i=0;

  // Allow digit, alpha and underscores
  while(isdigit(c) || isalpha(c) || '_'==c){
     if(lim-1==i){
       fatal("Identifier too long");
     }else if(i < lim -1){
       buf[i++] = c;
     }
     c = next();
  }
  putback(c);
  buf[i]='\0';
  return i;
}

static int keyword(char *s){

  switch(*s){
    case 'i':
	 if(!strcmp(s,"int")){
	    return (T_INT);	 
	 }
	 break;
    case 'p':
	 if(!strcmp(s,"print")){
	    return (T_PRINT);
	 }
	 break;  
  }
  return 0;
}

// Scan and return the next token found in the input 
// Return 1 if token valid, 0 if no tokens left.

int scan(struct token *t){
  int c,tokentype;
  c = skip();
  
  switch(c){
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
  case ';':
    t->token = T_SEMI;
    break;
  case '=':
    t->token = T_EQUALS;
    break;
  default:
    //if digit
    if(isdigit(c)){
      t->intvalue = scanint(c);
      t->token = T_INTLIT;
      break;
    }else if(isalpha(c) || '_' == c){
      scanident(c,Text,TEXTLEN);

      //if the keyword is recognised keyword, return that token
      if(tokentype = keyword(Text)){
        t->token = tokentype;
	break;
      }

      // Not a recognized keyword, return that token
      t->token = T_IDENT;
      break;    
    }
    fatalc("Unrecognized character",c);
  }
  // We have found a token
  return 1;

}
