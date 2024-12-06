#include "defs.h"
#define extern_
#include "data.h"
#undef extern_
#include "decl.h"
#include <errno.h>

//Intialize the gloabal variable
static void init(){
  Line = 1;
  Putback = '\n';
}

//Print usage if starterd incorrectly
static void usage(char *prog){
  fprintf(stderr,"Usage: %s infile\n",prog);
  exit(1);
}

void main(int argc,char *argv[]){
  struct ASTnode *n;
  if(argc!=2){
     usage(argv[0]);
  }
  init();

//Open the file and point the infile pointer to the beginning of the input file
  if((Infile = fopen(argv[1],"r")) == NULL ){
    fprintf(stderr,"Unable to open the file %s: %s \n",argv[1],strerror(errno));
    exit(1);
  }

  scan(&Token);//get the first token. It has to be a int
  n = binexpr();//Parse the expression
  printf("%d\n",interpretAST(n));
  exit(0);
}

