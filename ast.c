#include "ast.h"
#include <stdlib.h>
#include <string.h>


/* Static helpers */

/* Allocate new node and set its kind */
static Node node_new(int kind)
{
	Node n = malloc(sizeof(*n));
	n->kind = kind;
	return n;
}


/* Constructors */

/* Create numeric constant node */
Node node_num(double val)
{
	Node n = node_new(N_NUM);
	n->u.val = val;
	return n;
}

/* Create polynomial variable node */
Node node_pvar(char var)
{
	Node n = node_new(N_PVAR);
	n->u.var = var;
	return n;
}

/* Create identifier node */
Node node_ident(char *ident)
{
	Node n = node_new(N_IDENT);
	n->u.ident = strdup(ident);
	return n;
}

/* Create assignment node */
Node node_assign(char *ident, Node expr)
{
	Node n = node_new(N_ASSIGN);
	n->u.assign.ident = strdup(ident);
	n->u.assign.expr = expr;
	return n;
}

/* Create binary operation node */
Node node_binop(int op, Node left, Node right)
{
	Node n = node_new(N_BINOP);
	n->u.binop.op = op;
	n->u.binop.left = left;
	n->u.binop.right = right;
	return n;
}

/* Create unary operation node */
Node node_unop(int op, Node expr)
{
	Node n = node_new(N_UNOP);
	n->u.unop.op = op;
	n->u.unop.expr = expr;
	return n;
}
