#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABSIZE 127  /* hash table size - should be prime */

/* Binding - associates a variable name with its value. */
struct binding {
    char *name;
    double value;
    struct binding *next;  /* for collisions */
};

static struct binding *table[TABSIZE]; /* hash table */

/* SDBM hash function */
static unsigned int hash(char *s)
{
    unsigned int h = 0;
    for (; *s; s++)
        h = h * 65599 + *s;
    return h % TABSIZE;
}

static struct binding *lookup(char *name)
{
	struct binding *b;
    for (b = table[hash(name)]; b; b = b->next)
        if (strcmp(b->name, name) == 0)
            return b;
    return NULL;
}

double env_get(char *name)
{
    struct binding *b = lookup(name);
    if (!b) {
        fprintf(stderr, "semantic error: undefined variable '%s'\n", name);
        return 0;
    }
    return b->value;
}

void env_set(char *name, double val)
{
    struct binding *b = lookup(name);
    if (b) {
        b->value = val;
        return;
    }
	unsigned int h = hash(name);
    b = malloc(sizeof(*b));
    b->name = strdup(name);
    b->value = val;
    b->next = table[h];
    table[h] = b;
}
