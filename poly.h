#ifndef POLY_H
#define POLY_H

/* Structs/typedefs */
typedef struct poly *Polynomial;

/* Constructors */
Polynomial poly_const(double coeff);
Polynomial poly_var(char var);
Polynomial poly_copy(Polynomial p);

/* Destructor */
void poly_free(Polynomial p);

/* Core operations */
Polynomial poly_neg(Polynomial p);
Polynomial poly_add(Polynomial a, Polynomial b);
Polynomial poly_sub(Polynomial a, Polynomial b);
Polynomial poly_mul(Polynomial a, Polynomial b);
Polynomial poly_div(Polynomial a, Polynomial b);
Polynomial poly_pow(Polynomial a, unsigned int n);

/* Predicates */
int poly_is_int(Polynomial p, int *out);

/* Input/output */
void poly_print(Polynomial p);

#endif /* ifndef POLY_H */
