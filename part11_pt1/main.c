#include "defs.h"
#define extern_
#include "data.h"
#undef extern_
#include "decl.h"
#include <errno.h>

//Initialize the global variables.
static void init(){
  Line = 1;
  Putback = '\n';
}

//Print out a usage if started incorrectlr
static void usage(char *prog){
  fprintf(stderr,"Usage: %s infile\n",prog);
  exit(1);
}

// Main program: check arguments and print a usage 
// if we don't have an argument. Open up the input
// file and call scanfile() to scan the tokens in it.
void main(int argc,char *argv[]){
  struct ASTnode *tree;

  if (argc != 2)
    usage(argv[0]);

  init();

  // Open up the input file
  if ((Infile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  // Create the output file
  if ((Outfile = fopen("out.s", "w")) == NULL) {
    fprintf(stderr, "Unable to create out.s: %s\n", strerror(errno));
    exit(1);
  }

  scan(&Token);
  genpreamble();
  while(1){
    tree = function_declaration();
    genAST(tree,NOREG,0);
    if(Token.token == T_EOF){
	    break;
    }
  }
  fclose(Outfile);
  exit(0);

}

