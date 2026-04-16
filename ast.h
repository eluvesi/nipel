#ifndef AST_H
#define AST_H

typedef struct node *Node;

struct node {
	enum {
		N_CONST,   /* constants */
		N_VAR,     /* variables */
		N_ASSIGN,  /* assignments */
		N_BINOP,   /* binary operations */
		N_UNOP,    /* unary operations */
	} kind;

	union {
		double val;  /* N_CONST */

		char *name;  /* N_VAR */

		struct {
			char *name;
			Node expr;
		} assign;  /* N_ASSIGN */

		struct {
			enum { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW } oper;
			Node left;
			Node right;
		} binop;  /* N_BINOP */

		struct {
			enum { OP_NEG } oper;
			Node expr;
		} unop;  /* N_UNOP */

	} u;
};

Node node_const(double v);
Node node_var(char *name);
Node node_assign(char *name, Node expr);
Node node_binop(int op, Node left, Node right);
Node node_unop(int op, Node expr);

#endif /* ifndef AST_H */
