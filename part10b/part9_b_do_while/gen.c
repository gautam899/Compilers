#include "defs.h"
#include "data.h"
#include "decl.h"

// Generic code generator
// Copyright (c) 2019 Warren Toomey, GPL3

// Generate and return a new label number
static int label(void) {
  static int id = 1;
  return (id++);
}

// Generate the code for an IF statement
// and an optional ELSE clause
static int genIF(struct ASTnode *n) {
  int Lfalse, Lend;

  // Generate two labels: one for the
  // false compound statement, and one
  // for the end of the overall IF statement.
  // When there is no ELSE clause, Lfalse _is_
  // the ending label!
  Lfalse = label();
  if (n->right)
    Lend = label();

  // Generate the condition code followed
  // by a jump to the false label.
  // We cheat by sending the Lfalse label as a register.
  genAST(n->left, Lfalse, n->op);
  genfreeregs();

  // Generate the true compound statement
  genAST(n->mid, NOREG, n->op);
  genfreeregs();

  // If there is an optional ELSE clause,
  // generate the jump to skip to the end
  if (n->right)
    cgjump(Lend);

  // Now the false label
  cglabel(Lfalse);

  // Optional ELSE clause: generate the
  // false compound statement and the
  // end label
  if (n->right) {
    genAST(n->right, NOREG, n->op);
    genfreeregs();
    cglabel(Lend);
  }

  return (NOREG);
}
static int genDO_WHILE(struct ASTnode *n){
   //In do while we will only require one label.
   int Lstart;
   Lstart = label();
   cglabel(Lstart);
   //The body now
   genAST(n->right,NOREG,n->op);
   genfreeregs();
   //Now the condition part 
   genAST(n->left,Lstart,n->op);
   genfreeregs();
   return NOREG;

}
// Generate the code for a WHILE statement
// and an optional ELSE clause
static int genWHILE(struct ASTnode *n) {
  //int Lstart, Lend;

  // Generate the start and end labels
  // and output the start label
  //Lstart = label();
 // Lend = label();
  //cglabel(Lstart);

  // Generate the condition code followed
  // by a jump to the end label.
  // We cheat by sending the Lfalse label as a register.
  //genAST(n->left, Lend, n->op);
  //genfreeregs();

  // Generate the compound statement for the body
  //genAST(n->right, NOREG, n->op);
  //genfreeregs();

  // Finally output the jump back to the condition,
  // and the end label
  //cgjump(Lstart);
  //cglabel(Lend);
  //return (NOREG);
   
  int Lcomp,Lbody;
  Lcomp = label();
  Lbody = label();

  cgjump(Lcomp);
  cglabel(Lbody);//body starts here
  genAST(n->right,NOREG,n->op);
  genfreeregs();
  
  cglabel(Lcomp);
  genAST(n->left,Lbody,n->op);
  genfreeregs();
  return NOREG;
}

// Given an AST, the register (if any) that holds
// the previous rvalue, and the AST op of the parent,
// generate assembly code recursively.
// Return the register id with the tree's final value
int genAST(struct ASTnode *n, int reg, int parentASTop) {
  int leftreg, rightreg;

  // We now have specific AST node handling at the top
  switch (n->op) {
    case A_IF:
      return (genIF(n));
    case A_WHILE:
      return (genWHILE(n));
    case A_DO:
      return (genDO_WHILE(n));
    case A_GLUE:
      // Do each child statement, and free the
      // registers after each child
      genAST(n->left, NOREG, n->op);
      genfreeregs();
      genAST(n->right, NOREG, n->op);
      genfreeregs();
      return (NOREG);
  }

  // General AST node handling below

  // Get the left and right sub-tree values
  if (n->left)
    leftreg = genAST(n->left, NOREG, n->op);
  if (n->right)
    rightreg = genAST(n->right, leftreg, n->op);

  switch (n->op) {
    case A_ADD:
      return (cgadd(leftreg, rightreg));
    case A_SUBTRACT:
      return (cgsub(leftreg, rightreg));
    case A_MULTIPLY:
      return (cgmul(leftreg, rightreg));
    case A_DIVIDE:
      return (cgdiv(leftreg, rightreg));
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:
      // If the parent AST node is an A_IF or A_WHILE, generate
      // a compare followed by a jump. Otherwise, compare registers
      // and set one to 1 or 0 based on the comparison.
      if (parentASTop == A_IF || parentASTop == A_WHILE || parentASTop == A_DO)
	return (cgcompare_and_jump(n->op, leftreg, rightreg, reg));
      else
	return (cgcompare_and_set(n->op, leftreg, rightreg));
    case A_INTLIT:
      return (cgloadint(n->v.intvalue));
    case A_IDENT:
      return (cgloadglob(Gsym[n->v.id].name));
    case A_LVIDENT:
      return (cgstorglob(reg, Gsym[n->v.id].name));
    case A_ASSIGN:
      // The work has already been done, return the result
      return (rightreg);
    case A_PRINT:
      // Print the left-child's value
      // and return no register
      genprintint(leftreg);
      genfreeregs();
      return (NOREG);
    default:
      fatald("Unknown AST operator", n->op);
  }
}

void genpreamble() {
  cgpreamble();
}
void genpostamble() {
  cgpostamble();
}
void genfreeregs() {
  freeall_registers();
}
void genprintint(int reg) {
  cgprintint(reg);
}

void genglobsym(char *s) {
  cgglobsym(s);
}
