#include "ast.h"
#include "env.h"
#include <stdio.h>
#include <math.h>


double eval(Node t)
{
	switch (t->kind) {

	case N_CONST:
		return t->u.val;

	case N_VAR:
		return env_get(t->u.name);

	case N_ASSIGN: {
		double v = eval(t->u.assign.expr);
		env_set(t->u.assign.name, v);
		return v;
    }

	case N_BINOP: {
		double l = eval(t->u.binop.left);
		double r = eval(t->u.binop.right);

		switch (t->u.binop.oper) {
		case OP_ADD:
			return l + r;
		case OP_SUB:
			return l - r;
		case OP_MUL:
			return l * r;
		case OP_DIV:
			return l / r;
		case OP_POW: {
			if (r < 0) {
				fprintf(stderr, "semantic error: negative exponent\n");
				return 0;
			}
			double intpart;
			if (modf(r, &intpart) != 0.0) {
				fprintf(stderr, "semantic error: non-integer exponent\n");
				return 0;
			}
			return pow(l, r);
		}
		}
		break;
	}

	case N_UNOP:
		switch (t->u.unop.oper) {
			case OP_NEG:
				return -eval(t->u.unop.expr);
		}
		break;
	}

	return 0;
}
