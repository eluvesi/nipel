#include "ast.h"
#include <math.h>


double eval(Treeptr t)
{
    switch (t->kind) {
    case N_CONST:
        return t->u.val;
    case N_BINOP:
        switch (t->u.binop.oper) {
        case OP_ADD:
			return eval(t->u.binop.left) + eval(t->u.binop.right);
        case OP_SUB: 
			return eval(t->u.binop.left) - eval(t->u.binop.right);
        case OP_MUL: 
			return eval(t->u.binop.left) * eval(t->u.binop.right);
        case OP_DIV: 
			return eval(t->u.binop.left) / eval(t->u.binop.right);
        case OP_POW: 
			return pow(eval(t->u.binop.left), eval(t->u.binop.right));
        }
        break;
    case N_UNOP:
        switch (t->u.unop.oper) {
		case OP_NEG:
            return -eval(t->u.unop.expr);
		}
		break;
    }

    return 0;
}
