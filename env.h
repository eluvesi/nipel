#ifndef ENV_H
#define ENV_H

#include "poly.h"

/* Returns polynomial by its identifier */
Polynomial env_get(char *ident);

/* Creates new polynomial-identifier binding */
void env_set(char *ident, Polynomial poly);

#endif /* ifndef ENV_H */
