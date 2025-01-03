#ifndef extern_
#define extern_ extern
#endif

// Current line number
extern_ int Line;
// Character putback by the scanner
extern_ int Putback;
// Input and output file pointer
extern_ FILE *Infile;
extern_ FILE *Outfile;
// Last scanned Token
extern_ struct token Token;
// Last Identifier scanned
extern_ char Text[TEXTLEN+1];
// Global Symbol Table
extern_ struct symtable Gsym[NSYMBOLS];

