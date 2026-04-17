#ifndef AST_H
#define AST_H

typedef struct node *Node;

struct node {
	enum {
		N_NUM,     /* mumeric constants */
		N_PVAR,    /* polynomial variables */
		N_IDENT,   /* global variables */
		N_ASSIGN,  /* assignments */
		N_BINOP,   /* binary operations */
		N_UNOP,    /* unary operations */
	} kind;

	union {
		double val;   /* N_NUM */

		char var;     /* N_PVAR */

		char *ident;  /* N_IDENT */

		struct {
			char *ident;
			Node expr;
		} assign;  /* N_ASSIGN */

		struct {
			enum { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW } op;
			Node left;
			Node right;
		} binop;  /* N_BINOP */

		struct {
			enum { OP_NEG } op;
			Node expr;
		} unop;  /* N_UNOP */
	} u;
};

/* Constructors */
Node node_num(double val);
Node node_pvar(char var);
Node node_ident(char *ident);
Node node_assign(char *ident, Node expr);
Node node_binop(int op, Node left, Node right);
Node node_unop(int op, Node expr);

#endif /* ifndef AST_H */
