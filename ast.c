#include "ast.h"
#include <stdlib.h>
#include <string.h>


Node node_num(double val)
{
	Node n = malloc(sizeof(*n));
	n->kind = N_NUM;
	n->u.val = val;
	return n;
}

Node node_pvar(char var)
{
	Node n = malloc(sizeof(*n));
	n->kind = N_PVAR;
	n->u.var = var;
	return n;
}

Node node_ident(char *ident)
{
	Node n = malloc(sizeof(*n));
	n->kind = N_IDENT;
	n->u.ident = strdup(ident);
	return n;
}

Node node_assign(char *ident, Node expr)
{
	Node n = malloc(sizeof(*n));
	n->kind = N_ASSIGN;
	n->u.assign.ident = strdup(ident);
	n->u.assign.expr = expr;
	return n;
}

Node node_binop(int op, Node left, Node right)
{
	Node n = malloc(sizeof(*n));
	n->kind = N_BINOP;
	n->u.binop.op = op;
	n->u.binop.left = left;
	n->u.binop.right = right;
	return n;
}

Node node_unop(int op, Node expr)
{
	Node n = malloc(sizeof(*n));
	n->kind = N_UNOP;
	n->u.unop.op = op;
	n->u.unop.expr = expr;
	return n;
}
