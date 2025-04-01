#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure and enum definitions
// Copyright (c) 2019 Warren Toomey, GPL3

#define TEXTLEN		512	// Length of symbols in input
#define NSYMBOLS        1024	// Number of symbol table entries

// Token types
// If token is n we can look up 15+n.
enum {
  T_EOF, // Operator s
  T_ASSIGN,
  T_PLUS, 
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_EQ, 
  T_NE,
  T_LT, 
  T_GT, 
  T_LE, 
  T_GE, // Type keywords
  T_VOID, 
  T_CHAR, 
  T_INT, 
  T_LONG,// Structural token
  T_INTLIT, 
  T_SEMI,  
  T_IDENT,
  T_LBRACE, 
  T_RBRACE, 
  T_LPAREN, 
  T_RPAREN,
  T_LBRACKET,
  T_RBRACKET,
  T_AMPER, 
  T_LOGAND,
  T_PRINT,// Other keywords from the next line 
  T_IF, 
  T_ELSE, 
  T_WHILE, 
  T_DO,
  T_FOR, 
  T_RETURN
};

// Token structure
struct token {
  int token;			// Token type, from the enum list above
  int intvalue;			// For T_INTLIT, the integer value
};

// AST node types. The first few line up
// with the related tokens
// If the token n then we can look it up at 59+n-1;
enum {
  A_ASSIGN = 1,
  A_ADD,
  A_SUBTRACT, 
  A_MULTIPLY, 
  A_DIVIDE,
  A_EQ, 
  A_NE,
  A_LT, 
  A_GT, 
  A_LE, 
  A_GE,
  A_INTLIT,
  A_IDENT,  
  A_GLUE,
  A_IF, 
  A_WHILE,
  A_DO, 
  A_FUNCTION, 
  A_WIDEN, 
  A_RETURN,
  A_FUNCCALL,
  A_DEREF, 
  A_ADDR,
  A_SCALE
};

//Some primitive types
enum {
  P_NONE, 
  P_VOID, 
  P_CHAR, 
  P_INT, 
  P_LONG,
  P_VOIDPTR, 
  P_CHARPTR, 
  P_INTPTR, 
  P_LONGPTR
};
// Abstract Syntax Tree structure
struct ASTnode {
  int op;			// "Operation" to be performed on this tree
  int type;
  int rvalue;
  struct ASTnode *left;		// Left, middle and right child trees
  struct ASTnode *mid;
  struct ASTnode *right;
  union {
    int intvalue;		// For A_INTLIT, the integer value
    int id;    // For A_IDENT, the symbol slot number
    int size;
  } v;
};

#define NOREG	-1		// Use NOREG when the AST generation
                                // functions have no register to return
#define NOLABEL 0               // Use NOLABEL when we have no label to
				// pass to genAST()
// Structural types
enum {
  S_VARIABLE, S_FUNCTION, S_ARRAY
};				// functions have no register to return

// Symbol table structure
struct symtable {
  char *name;  // Name of a symbol
  int type;  // Primitive type for the symbol.
  int stype; // Structural type for the symbol
  int endlabel; // For S_FUNCTIONs, the end label
  int size; // Number of elements in the symbol
};
