#ifndef AST_H
#define AST_H

typedef struct tnode *Treeptr;

typedef struct tnode {
	enum {
		N_CONST,   /* constants */
		N_BINOP,   /* binary operations */
		N_UNOP,    /* unary operations */
	} kind;
	
	union {
		double val;  /* N_CONST */
		
		struct {
			enum { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW } oper;
			Treeptr left;
			Treeptr right;
		} binop;  /* N_BINOP */
		
		struct {
			enum { OP_NEG } oper;
			Treeptr expr;
		} unop;  /* N_UNOP */
		
	} u;
} Treenode;

Treeptr new_const(double v);
Treeptr new_binop(int op, Treeptr l, Treeptr r);
Treeptr new_unop(int op, Treeptr e);
void print_tree(Treeptr t);

#endif /* ifndef AST_H */
