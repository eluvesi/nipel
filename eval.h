#ifndef EVAL_H
#define EVAL_H

#include "ast.h"
#include "poly.h"

/* Evaluate AST into polynomial */
Polynomial eval(Node n);

#endif /* ifndef EVAL_H */
