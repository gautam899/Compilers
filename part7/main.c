#include "defs.h"
#define extern_
#include "data.h"
#undef extern_
#include "decl.h"
#include <errno.h>


// Initialize global variable
static void init(){
  Line = 1;
  Putback = '\n';
}

// Print out a usage if started incorrectly
static void usage(char *prog){
  fprintf(stderr,"Usage: %s infile\n",prog);
  exit(1);
}


void main(int argc,char *argv[]){
  if(argc != 2){
    usage(argv[0]);
  }

  init();
  
  // Open up the input file
  if((Infile = fopen(argv[1],"r")) == NULL){
    fprintf(stderr, "Unable to open %s: %s\n",argv[1],strerror(errno));
    exit(1);
  }

  // Create the output file
  if((Outfile = fopen("out.s","w")) == NULL){
    fprintf(stderr, "Unable to create out.s: %s\n",strerror(errno));
    exit(1);
  }

  scan(&Token); // Get the first token from the input 
  genpreamble(); // Output the preamble
  statements();
  genpostamble();
  fclose(Outfile);
  exit(0);


}
