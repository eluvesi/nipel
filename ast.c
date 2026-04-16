#include "ast.h"
#include <stdlib.h>
#include <string.h>


Node node_const(double v)
{
    Node n = malloc(sizeof(*n));
    n->kind = N_CONST;
    n->u.val = v;
    return n;
}

Node node_var(char *name)
{
    Node n = malloc(sizeof(*n));
    n->kind = N_VAR;
    n->u.name = strdup(name);
    return n;
}

Node node_assign(char *name, Node expr)
{
    Node n = malloc(sizeof(*n));
    n->kind = N_ASSIGN;
    n->u.assign.name = strdup(name);
    n->u.assign.expr = expr;
    return n;
}

Node node_binop(int op, Node left, Node right)
{
    Node n = malloc(sizeof(*n));
    n->kind = N_BINOP;
    n->u.binop.oper = op;
    n->u.binop.left = left;
    n->u.binop.right = right;
    return n;
}

Node node_unop(int op, Node expr)
{
    Node n = malloc(sizeof(*n));
    n->kind = N_UNOP;
    n->u.unop.oper = op;
    n->u.unop.expr = expr;
    return n;
}
