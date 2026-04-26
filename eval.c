#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "env.h"
#include "poly.h"


/* Evaluate AST node and return resulting polynomial */
Polynomial eval(Node n)
{
	switch (n->kind) {
	case N_NUM:
		/* return numeric constant polynomial */
		return poly_const(n->u.val);

	case N_PVAR:
		/* return one variable polynomial */
		return poly_var(n->u.var);

	case N_IDENT:
		/* lookup for identifier in environment */
		return env_get(n->u.ident);

	case N_ASSIGN: {
		/* evaluate right-hand side */
		Polynomial p = eval(n->u.assign.expr);
		/* store copy in environment */
		env_set(n->u.assign.ident, poly_copy(p));
		/* return original result */
		return p;
	}

	case N_BINOP: {
		/* evaluate and return binary operation result */
		Polynomial l, r, res;
		l = eval(n->u.binop.left);
		r = eval(n->u.binop.right);
		res = NULL;

		switch (n->u.binop.op) {
		case OP_ADD:
			/* addition */
			res = poly_add(l, r);
			break;

		case OP_SUB:
			/* subtraction */
			res = poly_sub(l, r);
			break;

		case OP_MUL:
			/* multiplication */
			res = poly_mul(l, r);
			break;

		case OP_DIV:
			/* division */
			res = poly_div(l, r);
			break;

		case OP_POW: {
			/* exponentiation */
			int exp;
			if(!poly_is_int(r, &exp)) {
				/* check that exponent is integer */
				fprintf(stderr, "semantic error: exponent must be integer\n");
				exit(1);
			}
			if (exp < 0) {
				/* check that exponent is positive */
				fprintf(stderr, "semantic error: exponent must be positive\n");
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

	case N_UNOP: {
		/* evaluate and return unary operation result */
		Polynomial x, res;
		x = eval(n->u.unop.expr);
		res = NULL;

		switch (n->u.unop.op) {
		case OP_NEG:
			/* unary minus */
			res = poly_neg(x);
			break;
		}

		poly_free(x);
		return res;
	}
	}

	return NULL;
}
