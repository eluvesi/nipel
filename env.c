#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poly.h"

#define TABSIZE 127  /* hash table size - should be prime */


/* Structs/typedefs */

/* Binding: associates a identifier with a polynomial. */
struct binding {
	char *ident;
	Polynomial poly;
	struct binding *next;  /* for collisions */
};

static struct binding *table[TABSIZE];  /* hash-table */


/* Static helpers */

/* SDBM hash-function */
static unsigned int hash(char *s)
{
	unsigned int h = 0;
	for (; *s; s++)
		h = h * 65599 + *s;
	return h % TABSIZE;
}

/* Try to find binding by its ident, return NULL on failure */
static struct binding *lookup(char *ident)
{
	struct binding *b;
	for (b = table[hash(ident)]; b; b = b->next)
		if (strcmp(b->ident, ident) == 0)
			return b;
	return NULL;
}


/* Environment API */

/* Returns polynomial by its identifier */
Polynomial env_get(char *ident)
{
	/* try to find binding for this ident */
	struct binding *b = lookup(ident);
	if (!b) {
		fprintf(stderr, "semantic error: undefined identifier '%s'\n", ident);
		exit(1);
	}

	/* return copy to avoid accidentally freeing memory for table elements */
	return poly_copy(b->poly);
}

/* Creates new identifier<-->polynomial binding */
void env_set(char *ident, Polynomial poly)
{
	/* try to find binding for this ident */
	struct binding *b = lookup(ident);

	/* if binding for this ident already exist */
	if (b) {
		/* replace old value (environment owns stored polynomial) */
		if(b->poly)
			poly_free(b->poly);
		b->poly = poly;
		return;
	}

	/* if this is new ident */
	unsigned int h = hash(ident);  /* calculate hash value */
	b = malloc(sizeof(*b));        /* allocate memory for binding */
	b->ident = strdup(ident);      /* copy ident string */
	b->poly = poly;                /* take ownership over poly */
	b->next = table[h];            /* current table[hash] is now b->next */
	table[h] = b;                  /* b is now new table[hash] */
}
