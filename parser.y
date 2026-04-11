%{
#include <stdio.h>

extern FILE *yyin;

int yylex(void);
void yyerror(const char *s);
%}

%defines "parser.h"
%output  "parser.c"

%token PVAR IDENT INTEGER FLOAT STRING NEWLINE
%token ASSIGN ADD SUB MUL DIV POW
%token LPAREN RPAREN DOLLAR

%%

program:
	  /* empty */
	| program statement
	;

statement:
	  assignment NEWLINE  { printf("=> assign\n"); }
	| expression NEWLINE  { printf("=> expr\n");   }
	| STRING NEWLINE      { printf("=> string\n"); }
	| NEWLINE
	;

assignment:
	  IDENT ASSIGN expression
	;

expression:
	  additive-expr
	;

additive-expr:
	  multiplicative-expr
	| additive-expr ADD multiplicative-expr
	| additive-expr SUB multiplicative-expr
	;

multiplicative-expr:
	  unary-expr
	| multiplicative-expr exponential-expr
	| multiplicative-expr MUL unary-expr
	| multiplicative-expr DIV unary-expr
	;

unary-expr:
	  exponential-expr
	| ADD exponential-expr
	| SUB exponential-expr
	;

exponential-expr:
	  primary
	| primary POW exponential-power
	;

exponential-power:
	  INTEGER
	| INTEGER POW exponential-power
	;

primary:
	  INTEGER
	| FLOAT
	| PVAR 
	| DOLLAR IDENT 
	| LPAREN expression RPAREN
	;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Parse error: %s\n", s);
}

int main (int argc, char *argv[])
{
	if (argc < 2 || (argv[1][0] == '-' && argv[1][1] == '\0'))
		yyin = stdin;

	else {
		yyin = fopen(argv[1], "r");
		if (!yyin) {
			perror(argv[1]);
			return 1;
		}
	}

	yyparse();

	if (yyin != stdin)
		fclose(yyin);

	return 0;
}
