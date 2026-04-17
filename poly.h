#ifndef POLY_H
#define POLY_H

typedef struct poly *Polynomial;

/* Constructors */
Polynomial poly_const(double coeff);
Polynomial poly_var(char var);

/* Algebra */
Polynomial poly_add(Polynomial a, Polynomial b);
Polynomial poly_sub(Polynomial a, Polynomial b);
Polynomial poly_mul(Polynomial a, Polynomial b);
Polynomial poly_div(Polynomial a, Polynomial b);
Polynomial poly_pow(Polynomial a, unsigned int n);
Polynomial poly_neg(Polynomial p);

/* Utilities */
Polynomial poly_copy(Polynomial p);
void poly_free(Polynomial p);
void poly_print(Polynomial p);
int poly_is_int(Polynomial p, int *out);

#endif /* ifndef POLY_H */
