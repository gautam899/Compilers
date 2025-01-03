#include "defs.h"
#include "data.h"
#include "decl.h"

// This file include all the symbol table functions

static int Globs = 0; //Position of the next free global sym slot


//If the symbol is in the symbol table or not. If yes return the slot id else return -1 if it does not exist.
int findglob(char *s){
   int i;
   for(i=0;i<Globs;i++){
     if(*s == *Gsym[i].name && !strcmp(s, Gsym[i].name)){
       return i;
     }
   }
   return -1;
}

//Find the postion for the new global symbol. If we run out of space. Die.
static int newglob(void){
   int p;
   if((p = Globs++) >= NSYMBOLS){
      fatal("Too many global symbols");
   }
   return p;
}


//Add global symbol to the symbol table. Return the slot number of the symbol table
int addglob(char *name){
   int y;
   
   //If already present in the global symbol table then return the existing slot.
   if((y = findglob(name)) != -1){
      return y;
   }

   y = newglob();
   Gsym[y].name = strdup(name);
   return y;
}
