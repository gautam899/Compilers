#include "defs.h"
#include "data.h"
#include "decl.h"
static int chrpos(char *s,int c){
  char *p;

  p = strchr(s,c);
  return (p ? p-s:-1);
}


static int next(){
  int c;
  //If we have a putback character in the stream then we return that first.
  if(Putback){
    c = Putback;
    Putback = 0;
    return c;
  }
  c = fgetc(Infile);//Read from the input file
  if(c == '\n'){
    Line++;
  }
  return c;

}
static void putback(int c){
   Putback = c;   
}

static int skip(void){
  int c;
  c = next();
  while(c ==' '|| '\t' == c || '\n' == c || '\r' == c || '\f' == c){
    c = next();//keep finding the next character untill we face these useless characters
  }
  return c;
}
static int scanint(int c){
   //From the ascii value c we need to find out what character does it belongs to.
   int k =0,val=0;
    
   //Build the whole number
   while((k = chrpos("0123456789",c)) >= 0){
      val = val*10+k;
      c = next();
   }
   //We hit a non-integer character, put it back in the stream to be read later on.
   putback(c);
   return val;
}




static int scanident(int c,char *buf, int lim){
   int i=0;
   //Allow digit
   while(isalpha(c) || isdigit(c) || '_' == c){
       if(lim -1 == i){
          fatal("Identifier too long");
       }
       else if(i<lim-1){
	    buf[i++] = c;
       }
       //Find the next token
       c = next();
   }
   putback(c);
   buf[i] = '\0';
   return i;
}



static int keyword(char *s){
  switch(*s){
     case 'i':
	  if(!strcmp(s,"int")){
	    return T_INT;
	  }
	  break;
     case 'p':
	  if(!strcmp(s,"print")){
	    return T_PRINT;
	  }
	  break;
  }
  return 0;

}
int scan(struct token *t){
   int c,tokentype;
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
      case ';':
	   t->token = T_SEMI;
	   break;
      case '=':
	   if((c = next()) == '='){
	        t->token = T_EQ;
	   }else{
		putback(c);
                t->token = T_ASSIGN;
	   }
	   break;
      case '!':
	   if((c = next()) == '='){
	   	t->token = T_NE;	   
	   }
	   else{
		fatalc("Unrecognized character ",c);
	   }
	   break;
      case '>':
	   if((c = next()) == '=') {
	       t->token = T_GE;
	   } 
	   else{
	       putback(c);
	       t->token = T_GT;
	   }
	   break;
      case '<':
	   if((c = next()) == '='){
	       t->token = T_LE;
	   }
	   else{ 
	       putback(c);
	       t->token = T_LT;
	   }
	   break;
      default: 
           
          if(isdigit(c)){
	     t->intvalue = scanint(c);
             t->token = T_INTLIT;
	     break;
	  }	   
	  else if(isalpha(c) || '_' == c){
             scanident(c,Text,TEXTLEN);
             //if this token is a recognized keyword
	     if((tokentype = keyword(Text))){
	        t->token = tokentype;
		break;
	     }

	     //if not a valid keyword then it is a identifier
	     t->token = T_IDENT;
	     break;  
	  }
	  fatalc("Unrecognized character character",c);
   }
   //we found a token
   return 1;

}
