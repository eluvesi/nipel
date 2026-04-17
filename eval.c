#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "env.h"
#include "poly.h"


Polynomial eval(Node n)
{
	switch (n->kind) {

	case N_NUM:
		return poly_const(n->u.val);

	case N_PVAR:
		return poly_var(n->u.var);

	case N_IDENT:
		return env_get(n->u.ident);

	case N_ASSIGN: {
		Polynomial p = eval(n->u.assign.expr);
		env_set(n->u.assign.ident, poly_copy(p));  /* env gets its own copy */
		return p;  /* caller gets original */
	}

	case N_BINOP: {
		Polynomial l = eval(n->u.binop.left);
		Polynomial r = eval(n->u.binop.right);
		Polynomial res = NULL;
		switch (n->u.binop.op) {
		case OP_ADD:
			res = poly_add(l, r);
			break;
		case OP_SUB:
			res = poly_sub(l, r);
			break;
		case OP_MUL:
			res = poly_mul(l, r);
			break;
		case OP_DIV:
			res = poly_div(l, r);
			break;
		case OP_POW: {
			int exp;
			if(!poly_is_int(r, &exp)) {
				fprintf(stderr,
				        "semantic error: exponent must be integer\n");
				exit(1);
			}
			if (exp < 0) {
				fprintf(stderr,
				        "semantic error: exponent must be positive\n");
				exit(1);
			}
			res = poly_pow(l, (unsigned int)exp);
			break;
		}
		}
		poly_free(l);
		poly_free(r);
		return res;
	}

	case N_UNOP:
		switch (n->u.unop.op) {
		case OP_NEG: {
			Polynomial x = eval(n->u.unop.expr);
			Polynomial res = poly_neg(x);
			poly_free(x);
			return res;
		}
		}
		break;
	}

	return 0;
}
