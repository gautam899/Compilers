#include "defs.h"
#include "data.h"
#include "decl.h"

//Position of the next free global symbol slot
static int Globs = 0;

// Determine if the symbol s is in the global symbol table.
// Return the slot position
int findglob(char *s){
  int i;
  for(i=0;i<Globs;i++){
     if(*s == *Gsym[i].name && !strcmp(s,Gsym[i].name)){
       return i;
     }
  }
  return -1;
}

// Get the position of the new global symbol slot, or die
// if we've run out of postions
static int newglob(){
  int p;
  if((p=Globs++) >= NSYMBOLS){
    fatal("Too many global symbols");
  }
  return p;
}

// Add a global symbol to the symbol table.
// Return the slot number in the 
int addglob(char *name){
  int y;
  
  // If the symbol already exist in the symbol table, return the existing slot
  if((y = findglob(name)) != -1){
     return y;
  }

  // Otherwise get a new slot, fill it in and return the slot number
  y = newglob();
  Gsym[y].name = strdup(name);
  return y;
}


