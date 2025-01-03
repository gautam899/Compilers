#include "defs.h"
#include "data.h"
#include "decl.h"

//Size 4 since we have 4 operations.
static int freereg[3];
static char *reglist[3] = {"%r8","%r9","%r10"};


//function to free all the register.
void freeall_registers(void){
  freereg[0]= freereg[1]= freereg[2]= 1;
}

//allocate a free register and return register identifier. Die if no register available.
static int alloc_register(void){
  for(int i=0;i<3;i++){
    if(freereg[i]){
      freereg[i]=0;
      return i;
    }
  }
  fprintf(stderr,"Sorry, we are out of register!\n");
  exit(1);
}

static void free_register(int reg){
  if(freereg[reg] != 0){
    fprintf(stderr,"Error trying to free register %d\n",reg);
    exit(1);
  }
  freereg[reg]=1;
}

//We will need to wrap the assembly code that we generate with some leading code and end it with traling code. For that we will use fputc as it write a string to a file.
void cgpreamble(){
  freeall_registers();
  fputs(
        "\t.text\n"
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
	"main:\n"
	"\tpushq\t%rbp\n"
	"\tmovq	%rsp, %rbp\n",
        Outfile	  
    );   
}

void cgpostamble(){
  fputs(
        "\tmovl $0, %eax\n"
        "\tpopq %rbp\n"
        "\tret\n",
        Outfile);
}


//Load a integer value in a register and then return the register number

int cgload(int value){
  //Get a new register
  int r = alloc_register();

  //Print out the code to intialize it
  fprintf(Outfile,"\tmovq\t$%d, %s\n",value,reglist[r]);
  return r;
}


int cgadd(int r1,int r2){
  fprintf(Outfile,"\taddq\t%s, %s\n",reglist[r1],reglist[r2]);
  free_register(r1);
  return (r2);
}

// Subtract the second register from the first and
// return the number of the register with the result
int cgsub(int r1, int r2) {
  fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  free_register(r2);
  return(r1);
}

//Multiply the two register and return the number of register of the register with the result.
int cgmul(int r1, int r2){
  fprintf(Outfile,"\timulq\t%s, %s\n",reglist[r1],reglist[r2]);
  // After multiplying the result will be stored in r2 so we can free r1
  free_register(r1);
  return r2;// r2 will hold the resulting value
}

//divide the first register by the second register and return the number of the register with the result.
int cgdiv(int r1,int r2){
  fprintf(Outfile,"\tmovq\t%s,%%rax\n",reglist[r1]);
  fprintf(Outfile,"\tcqo\n");//increment the stack by 8
  fprintf(Outfile,"\tidivq\t%s\n",reglist[r2]);
  fprintf(Outfile,"\tmovq\t%%rax, %s\n",reglist[r1]);
  free_register(r2);
  return r1;
}

//Calling the printint function with the given register
void cgprintint(int r){
  fprintf(Outfile,"\tmovq\t%s, %%rdi\n",reglist[r]);
  fprintf(Outfile,"\tcall\tprintint\n");
  free_register(r);
}





