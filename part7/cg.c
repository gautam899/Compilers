#include "defs.h"
#include "data.h"
#include "decl.h"

//Assembly code generator for x86-64

// List of available registers and their names
// We need a list of byte register, too

static int freereg[4];
static char *reglist[4] = {"%r8","%r9","%r10","%r11"};
static char *breglist[4] = {"%r8b","%r9b","%r10b","%r11b"};


// Set all registers as available
void freeall_registers(void){
  freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

// Allocate a free registers. Return the number of register and die if no free register avaible.
static int alloc_register(void){
  for(int i=0;i<4;i++){
    if(freereg[i]){
      freereg[i]=0;//make the register occupied
      return i;
    }
  }
  fatal("Out of register");
}

// Return a register to the list of available registers.
// Check to see if its not already there.
static void free_register(int reg){
   if(freereg[reg] != 0){
     fatald("Error trying to free register",reg);
   }
   freereg[reg] = 1;
}

// Print out the assembly preamble
void cgpreamble() {
  freeall_registers();
  fputs("\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"\t.globl\tmain\n"
	"\t.type\tmain, @function\n"
	"main:\n" "\tpushq\t%rbp\n" "\tmovq	%rsp, %rbp\n", Outfile);
}

// Print out the assembly postamble
void cgpostamble() {
  fputs("\tmovl	$0, %eax\n" "\tpopq	%rbp\n" "\tret\n", Outfile);
}

//Load an integer value into a register and return the register number.
int cgloadint(int value){
  int r = alloc_register();

  //Print out the code in the Outfile to initialize it 
  fprintf(Outfile,"\tmovq\t$%d, %s\n",value,reglist[r]);
  return r;
}

//Load value from a variable to a register
int cgloadglob(char *identifier){
  int r = alloc_register();
  
  fprintf(Outfile,"\tmovq\t%s(\%%rip), %s\n",identifier,reglist[r]);
  return r;
}

// Add two registers together and return
// the number of the register with the result
int cgadd(int r1, int r2) {
  fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return (r2);
}

// Subtract the second register from the first and
// return the number of the register with the result
int cgsub(int r1, int r2) {
  fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  free_register(r2);
  return (r1);
}

// Multiply two registers together and return
// the number of the register with the result
int cgmul(int r1, int r2) {
  fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return (r2);
}

// Divide the first register by the second and
// return the number of the register with the result
int cgdiv(int r1, int r2) {
  fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
  fprintf(Outfile, "\tcqo\n");
  fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
  fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
  free_register(r2);
  return (r1);
}


// Call printint() with the given register
void cgprintint(int r) {
  fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  fprintf(Outfile, "\tcall\tprintint\n");
  free_register(r);
}
// Store a register's value into a variable
int cgstorglob(int r, char *identifier) {
  fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], identifier);
  return (r);
}
// Generate a global symbol
void cgglobsym(char *sym) {
  fprintf(Outfile, "\t.comm\t%s,8,8\n", sym);
}

// Compare two registers.
static int cgcompare(int r1, int r2, char *how) {
  fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
  fprintf(Outfile, "\t%s\t%s\n", how, breglist[r2]);
  fprintf(Outfile, "\tandq\t$255,%s\n", reglist[r2]);
  free_register(r1);
  return (r2);
}

int cgequal(int r1, int r2) { return(cgcompare(r1, r2, "sete")); }
int cgnotequal(int r1, int r2) { return(cgcompare(r1, r2, "setne")); }
int cglessthan(int r1, int r2) { return(cgcompare(r1, r2, "setl")); }
int cggreaterthan(int r1, int r2) { return(cgcompare(r1, r2, "setg")); }
int cglessequal(int r1, int r2) { return(cgcompare(r1, r2, "setle")); }
int cggreaterequal(int r1, int r2) { return(cgcompare(r1, r2, "setge")); }
