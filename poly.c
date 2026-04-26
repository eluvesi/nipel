#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_VARS 26
#define EPSILON 1e-12


/* Structs/typedefs */

/* Monomial: coeff * a^powers[0] * b^powers[1] * ... * z^powers[25] */
typedef struct {
	double coeff;
	unsigned int degree;  /* total degree of monomial (sum of powers) */
	unsigned int powers[N_VARS];  /* degrees of variables */
} Monomial;

/* Polynomial: dynamic array of monomials */
struct poly {
	unsigned int size;
	unsigned int cap;
	Monomial *terms;
};


/* Static helpers */

/* Checks if a polynomial is a zero */
static int poly_is_zero(Polynomial p)
{
	return (p->size == 1 && p->terms[0].degree == 0 &&
	        fabs(p->terms[0].coeff) < EPSILON);
}

/* Monomial compare function for qsort (GRLEX) */
static int monomial_cmp(const void *a, const void *b)
{
	const Monomial *m1 = (const Monomial *)a;
	const Monomial *m2 = (const Monomial *)b;

	/* compare monomials by total degree */
	if (m1->degree != m2->degree)
		return (m1->degree < m2->degree) ? 1 : -1;  /* from highest to lowest */

	/* if sums are equal, compare degrees in lexicographic order */
	for (int i = 0; i < N_VARS; i++) {
		if (m1->powers[i] != m2->powers[i])
			return m1->powers[i] < m2->powers[i] ? 1 : -1;
	}

	/* if monomials are equal, return zero */
	return 0;
}

/*
 * Normalize polynomial to canonical form (merge identical monomials)
 *
 * Sorts monomials and merges identical ones
 * Removes zero coefficients and combines similar monomials
 */
static void poly_normalize(Polynomial p)
{
	if (!p || p->size <= 1)
		return;

	/* sort monomials using GRLEX order */
	qsort(p->terms, p->size, sizeof(Monomial), monomial_cmp);

	/* merge identical monomials */
	unsigned int write = 0;
	for (unsigned int read = 0; read < p->size; read++) {
		Monomial *cur = &p->terms[read];
		/* skip zero coefficients */
		if (fabs(cur->coeff) < EPSILON)
			continue;
		/* if same as previous monomial, merge */
		if (write > 0) {
			Monomial *last = &p->terms[write - 1];
			if (last->degree == cur->degree &&
			    memcmp(last->powers, cur->powers, sizeof(cur->powers)) == 0)
			{
				last->coeff += cur->coeff;
				/* if becomes zero, remove */
				if (fabs(last->coeff) < EPSILON)
					write--;
				continue;
			}
		}
		/* otherwise copy forward */
		if (write != read)
			p->terms[write] = *cur;
		write++;
	}

	if (write > 0)
		/* if at least one term remains */
		p->size = write;
	else {
		/* if all coeffs were 0, we need to create one zero-monomial manually */
		p->size = 1;
		p->terms[0].coeff = 0;
		for (int i = 0; i < N_VARS; i++)
			p->terms[0].powers[i] = 0;
		p->terms[0].degree = 0;
	} 
}


/* Constructors */

/* Creates a polynomial representing numeric constant */
Polynomial poly_const(double coeff)
{
	Polynomial p = malloc(sizeof(*p));  /* allocate polynomial struct */
	p->size = 1;  /* actual size is one monomial */
	p->cap = 1;   /* and capacity is the same */
	p->terms = malloc(p->cap * sizeof(*p->terms));  /* allocate this monomial */ 
	p->terms[0].coeff = coeff;  /* set coefficient */
	memset(p->terms[0].powers, 0, sizeof(p->terms[0].powers)); /* no vars */
	p->terms[0].degree = 0;  /* total degree is zero */
	return p;
}

/* Creates a polynomial representing a single variable */
Polynomial poly_var(char var)
{
	Polynomial p = poly_const(1.0);     /* start with constant 1 */
	p->terms[0].powers[var - 'a'] = 1;  /* set corresponding variable power */
	p->terms[0].degree = 1;             /* total degree is 1 */
	return p;
}

/* Creates a deep copy of polynomial */
Polynomial poly_copy(Polynomial p)
{
	if(!p)
		return NULL;

	Polynomial res = malloc(sizeof(*res));  /* allocate new polynomial */
	res->size = p->size;
	res->cap = p->size;  /* capacity equals actual size to save memory */

	/* allocate terms array and copy each monomial */
	res->terms = malloc(res->cap * sizeof(*res->terms));
	for (unsigned int i = 0; i < p->size; i++) {
		res->terms[i].coeff = p->terms[i].coeff;
		res->terms[i].degree = p->terms[i].degree;
		memcpy(res->terms[i].powers, p->terms[i].powers,
		       sizeof(p->terms[i].powers));
	}

	return res;
}


/* Destructor */

/* Frees memory allocated for polynomial */
void poly_free(Polynomial p)
{
	if (!p)
		return;
	free(p->terms);  /* free monomial array */
	free(p);         /* free polynomial struct */
}


/* Core operations */

/* Negate polynomial: returns polymonial -p */
Polynomial poly_neg(Polynomial p)
{
	if (!p)
		return NULL;

	/* copy polynomial 'a' to new polynomial 'res' */
	Polynomial res = poly_copy(p);

	/* negate all coefficients*/
	for (unsigned int i = 0; i < res->size; i++)
		res->terms[i].coeff = -res->terms[i].coeff;

	return res;
}

/* Addition of two polynomials: returns polynomial (a + b) */
Polynomial poly_add(Polynomial a, Polynomial b)
{
	if (!a || !b)
		return NULL;

	/* copy polynomial 'a' to new polynomial 'res' */
	Polynomial res = poly_copy(a);

	/* grow array if needed to fit all terms */
	unsigned int new_size = res->size + b->size;
	if (res->cap < new_size) {
		res->cap = new_size;
		res->terms = realloc(res->terms, res->cap * sizeof(*res->terms));
	}

	/* append to 'res' all monomials from 'b' */
	for (unsigned int i = 0; i < b->size; i++)
		res->terms[res->size++] = b->terms[i];

	/* normalize to canonical form */
	poly_normalize(res);

	return res;
}

/* Subtracting polynomial 'b' from polynomial 'a' */
Polynomial poly_sub(Polynomial a, Polynomial b)
{
	if (!a || !b)
		return NULL;

	/* copy polynomial 'a' to new polynomial 'res' */
	Polynomial res = poly_copy(a);

	/* grow array if needed to fit all terms */
	unsigned int new_size = res->size + b->size;
	if (res->cap < new_size) {
		res->cap = new_size;
		res->terms = realloc(res->terms, res->cap * sizeof(*res->terms));
	}

	/* negate all monomials from 'b' and append them to 'res' */
	for (unsigned int i = 0; i < b->size; i++) {
		Monomial m = b->terms[i];
		m.coeff = -m.coeff;
		res->terms[res->size++] = m;
	}

	/* normalize to canonical form */
	poly_normalize(res);

	return res;
}

/* Multiplication of two polynomials */
Polynomial poly_mul(Polynomial a, Polynomial b)
{
	if (!a || !b)
		return NULL;

	/* if 'a' is constant, we can simply multiply 'b' by it */
	if (a->size == 1 && a->terms[0].degree == 0) {
		double c = a->terms[0].coeff;
		/* if it is a zero, return zero polnomial */
		if (fabs(c) < EPSILON)
			return poly_const(0);
		/* if it is a one, return 'b' copy */
		if (fabs(c - 1.0) < EPSILON)
			return poly_copy(b);
		/* else multiply 'b' by this constant */
		Polynomial res = poly_copy(b);
		for (unsigned int i = 0; i < res->size; i++)
			res->terms[i].coeff *= c;
		return res;
	}

	/* absolutely symmetrical for 'b' */
	if (b->size == 1 && b->terms[0].degree == 0) {
		double c = b->terms[0].coeff;
		if (fabs(c) < EPSILON)
			return poly_const(0);
		if (fabs(c - 1.0) < EPSILON)
			return poly_copy(a);
		Polynomial res = poly_copy(a);
		for (unsigned int i = 0; i < res->size; i++)
			res->terms[i].coeff *= c;
		return res;
	}

	/* otherwise, allocate memory for multiplication result polynomial ...  */
	Polynomial res = malloc(sizeof(*res));
	res->size = 0;
	res->cap = a->size * b->size;  /* maximum possible terms */
	res->terms = malloc(res->cap * sizeof(*res->terms));

	/* ... and multiply all monomials from polynomials 'a' and 'b' */
	for (unsigned int i = 0; i < a->size; i++) {
		if (fabs(a->terms[i].coeff) < EPSILON)
			continue;  /* skip zero coeffs */
		for (unsigned int j = 0; j < b->size; j++) {
			if (fabs(b->terms[j].coeff) < EPSILON)
				continue;  /* skip zero coeffs */

			Monomial m;
			m.coeff = a->terms[i].coeff * b->terms[j].coeff;
			m.degree = a->terms[i].degree + b->terms[j].degree;
			for (int k = 0; k < N_VARS; k++)
				m.powers[k] = a->terms[i].powers[k] + b->terms[j].powers[k];

			res->terms[res->size++] = m;
		}
	}

	/* normalize to canonical form */
	poly_normalize(res);

	return res;
}

/* Raises the polynomial 'p' to the power 'n'' */
Polynomial poly_pow(Polynomial p, unsigned int n)
{
	if (!p)
		return NULL;

	/* if 'n' is 0 and 'p' is not 0, simply return 1.0 polynomial */
	if (n == 0) {
		if (poly_is_zero(p)) {
			fprintf(stderr, "semantic error: 0^0 is undefined\n");
			exit(1);
		}
		return poly_const(1.0);
	}

	/* if 'n' is 1, simply return 'p' copy */
	if (n == 1)
		return poly_copy(p);

	/* if 'p' is constant, we can simply use pow() from math.h */
	if (p->size == 1 && p->terms[0].degree == 0) {
		double c = p->terms[0].coeff;
		/* if it is a zero, return 0.0 polynomial */
		if (fabs(c) < EPSILON)
			return poly_const(0);
		/* if it is a one, return 1.0 polynomial */
		if (fabs(c - 1.0) < EPSILON)
			return poly_const(1.0);
		/* for other constants return pow(c, n) polynomial */
		return poly_const(pow(c, (double)n));
	}

	/* otherwise, use binary exponentiation algorithm */
	Polynomial res = poly_const(1.0);  /* result starts as 1 */
	Polynomial base = poly_copy(p);    /* working copy of base polynomial */

	while(n > 0) {
		/* if current bit of exponent is 1, multiply by base */
		if (n & 1) {  
			Polynomial tmp = poly_mul(res, base);
			poly_free(res);
			res = tmp;
		}

		n >>= 1;  /* move to next bit of exponent */

		/* if we are not done, square the base for next bit */
		if (n > 0) {  
			Polynomial tmp = poly_mul(base, base);
			poly_free(base);
			base = tmp;
		}
	}

	/* free temporary base polynomial */
	poly_free(base);

	return res;
}

/*
 * Division of polynomial 'a' by polynomial 'b'
 *
 * Performs polynomial long division:
 * returns quotient q such that a = q*b + r
 * remainder r is discarded
 */
Polynomial poly_div(Polynomial a, Polynomial b)
{
	if (!a || !b)
		return NULL;

	/* division by zero check */
	if (poly_is_zero(b)) {
		fprintf(stderr, "semantic error: division by zero\n");
		exit(1);
	}

	Polynomial q = poly_const(0.0);  /* quotient */
	Polynomial r = poly_copy(a);     /* remainder */

	while (!poly_is_zero(r)) {
		/* take leading terms of remainder and divisor */
		Monomial lt_r = r->terms[0];
		Monomial lt_b = b->terms[0];

		/* check if leading term lt_r is divisible by lt_b */
		int divisible = 1;
		for (int i = 0; i < N_VARS; i++) {
			if (lt_r.powers[i] < lt_b.powers[i]) {
				divisible = 0;
				break;
			}
		}
		if (!divisible)
			break;

		/* compute monomial m = lt_r / lt_b */
		Monomial m;
		m.coeff = lt_r.coeff / lt_b.coeff;
		m.degree = lt_r.degree - lt_b.degree;
		for (int i = 0; i < N_VARS; i++)
			m.powers[i] = lt_r.powers[i] - lt_b.powers[i];

		/* convert monomial 'm' to polynomial 't' */
		Polynomial t = poly_const(m.coeff);
		t->terms[0] = m;

		/* add to quotient: q += t */
		Polynomial tmp_q = poly_add(q, t);
		poly_free(q);
		q = tmp_q;

		/* subtract (t * b) from remainder: r -= t * b */
		Polynomial tb = poly_mul(t, b);
		Polynomial tmp_r = poly_sub(r, tb);
		poly_free(t);
		poly_free(tb);
		poly_free(r);
		r = tmp_r;	
	}

	poly_free(r);  /* remainder not returned, so free allocated memory */
	return q;      /* quotient returns */
}


/* Predicates */

/* Checks that a polynomial is an integer, if it is - sets 'out' value to it */
int poly_is_int(Polynomial p, int *out)
{
	if (!p || p->size != 1)
		return 0;  /* polynomial doesn't exist or have more than 1 monomial */
	if (p->terms[0].degree != 0)
		return 0;  /* first monomial isn't constant */
	if (fabs(p->terms[0].coeff - round(p->terms[0].coeff)) > EPSILON)
		return 0;  /* first monomial's coeff isn't integer */

	/* otherwise polynomial is integer constant */
	*out = (int)round(p->terms[0].coeff);  /* set out to this integer value */
	return 1;
}


/* Input/output */

/* Prints polynomial in human-readable form */
void poly_print(Polynomial p)
{
	if (!p)
		return;

	for (unsigned int i = 0; i < p->size; i++) {
		Monomial m = p->terms[i];
		double c = m.coeff;

		/* print sign between terms (handle first term separately) */
		if (i > 0) {
			if (c > 0)
				printf(" + ");
			else {
				printf(" - ");
				c = -c;  /* make coefficient positive for printing */
			}
		} else {
			/* first term: only print '-' if negative */
			if (c < 0) {
				printf("-");
				c = -c;  /* make coefficient positive for printing */
			}
		}

		/* coeff always prints for constants or when coeff != 1 */
		if (m.degree == 0 || fabs(c - 1.0) > EPSILON)
			printf("%g", c);

		/* print variables in lexicographic order (a..z) */
		for (int j = 0; j < N_VARS; j++) {
			unsigned int pow = m.powers[j];
			if (pow == 0)
				continue;
			printf("%c", 'a' + j);   /* print variable name */
			if (pow > 1)
				printf("^%u", pow);  /* print exponent if greater than 1 */
		}
	}
}
