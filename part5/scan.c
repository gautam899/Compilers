#include "defs.h"
#include "data.h"
#include "decl.h"

// Lexical scanning
// Copyright (c) 2019 Warren Toomey, GPL3

// Return the position of character c
// in string s, or -1 if c not found
static int chrpos(char *s, int c) {
  char *p;

  p = strchr(s, c);
  return (p ? p - s : -1);
}

// Get the next character from the input file.
static int next(void) {
  int c;

  if (Putback) {		// Use the character put
    c = Putback;		// back if there is one
    Putback = 0;
    return (c);
  }

  c = fgetc(Infile);		// Read from input file
  if ('\n' == c)
    Line++;			// Increment line count
  return (c);
}

// Put back an unwanted character
static void putback(int c) {
  Putback = c;
}

// Skip past input that we don't need to deal with, 
// i.e. whitespace, newlines. Return the first
// character we do need to deal with.
static int skip(void) {
  int c;

  c = next();
  while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
    c = next();
  }
  return (c);
}

// Scan and return an integer literal
// value from the input file.
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

// Scan Identifier
static void scanident(int c,char *buf,int lim){

  int i=0; // Length count
  
  // Build the identifier using a while loop
  while(isalpha(c) || isdigit(c) || '_' == c){
    // Error if hit the identifier length limit,
    // else append to buff[] and get the next
    if(lim-1 == i){
      printf("Identifier too long on line %d\n",Line);
      exit(1);
    }
    else if(i < lim - 1){
      buf[i++] = c;
    }
    c = next();
  }
  // We hit a non-valid character, put it back.
  // NUL-terminate the buf[] and return the length
  putback(c);
  buf[i] = '\0';
  //return (i);
}

static int keyword(char *s){
  switch(*s){
    case 'p':
        if(!strcmp(s,"print")){
	  return (T_PRINT);
	}
	break;
  }
  return 0;
}
int scan(struct token *t){

  int c,tokentype;

  // Skip the whitespaces
  c = skip();

  // Determine the token based on the input character
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
    default:

      //If a digit
      if(isdigit(c)){
        t->intvalue = scanint(c);
	t->token = T_INTLIT;
	break;
      }
      else if(isalpha(c) || '_' == c){
        // If not a digit then scan the keyword or identifier.
	scanident(c, Text, TEXTLEN);

	// If it is a recognized keyword, then return the token
	if(tokentype = keyword(Text)){
	   t->token = tokentype;
	   break;
	}
        
        // If not a recognized token then return an error
	printf("Unrecognized symbol %s on line number %d\n",Text,Line);
        exit(1);
 
      }

      //If the character isn't part of any recognized token, error
      printf("Unrecognised character %c on line %d\n",c,Line);
      exit(1);
  
  }
  //Found a token
  return (1);
}
