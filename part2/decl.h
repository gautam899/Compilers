//All the function declaration in this file will be treated as extern that is they can be redeclared in some other file but will be defined exactly once.
int scan(struct token *t);
struct ASTnode *mkastnode(int op,struct ASTnode *left, struct ASTnode *right, int intvalue);
//function to create leaf node
struct ASTnode *mkastleaf(int op, int intvalue);
//unary node with only one pointer.
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);
struct ASTnode *binexpr(void);
int interpretAST(struct ASTnode *n);

