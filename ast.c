#include "ast.h"
#include <stdlib.h>


Treeptr new_const(double v)
{
    Treeptr t = malloc(sizeof(*t));
    t->kind = N_CONST;
    t->u.val = v;
    return t;
}

Treeptr new_binop(int op, Treeptr l, Treeptr r)
{
    Treeptr t = malloc(sizeof(*t));
    t->kind = N_BINOP;
    t->u.binop.oper = op;
    t->u.binop.left = l;
    t->u.binop.right = r;
    return t;
}

Treeptr new_unop(int op, Treeptr e)
{
    Treeptr t = malloc(sizeof(*t));
    t->kind = N_UNOP;
    t->u.unop.oper = op;
    t->u.unop.expr = e;
    return t;
}
