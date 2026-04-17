#ifndef AST_H
#define AST_H

/* Structs/typedefs */
typedef struct node *Node;  /* Pointer to node struct */

struct node {  /* Abstract syntax tree node structure */
	enum {
		N_NUM,     /* numeric constants */
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

/* Constructors  */
Node node_num(double val);                       /* numeric constant node */
Node node_pvar(char var);                        /* polynomial variable node */
Node node_ident(char *ident);                    /* identifier node */
Node node_assign(char *ident, Node expr);        /* assignment node */
Node node_binop(int op, Node left, Node right);  /* binary operation node */
Node node_unop(int op, Node expr);               /* unary operation node */

#endif /* ifndef AST_H */
