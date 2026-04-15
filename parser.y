%{
#include <stdio.h>
#include <math.h>

extern FILE *yyin;
extern int yylineno;
extern char *yytext;

int yylex(void);
void yyerror(const char *s);
%}

%defines "parser.h"
%output  "parser.c"

%union {
	long ival;
	double dval;
	char *sval;
}

%token <ival> INTEGER
%token <dval> FLOAT
%token <sval> STRING IDENT PVAR
%token ASSIGN ADD SUB MUL DIV POW
%token LPAREN RPAREN DOLLAR NEWLINE

%type <dval> expression term factor power exponent primary

%%

program:
	  /* empty */
	| program statement
	;

statement:
	  assignment NEWLINE
	| expression NEWLINE  { printf("=> %g\n", $1);   }
	| STRING NEWLINE      { printf("=> %s\n", $1); }
	| NEWLINE
	;

assignment:
	  IDENT ASSIGN expression  { printf("=> %s = %g\n", $1, $3); }
	;

expression:
	  expression ADD term  { $$ = $1 + $3; }
	| expression SUB term  { $$ = $1 - $3; }
	| term                 { $$ = $1; }
	;

term:
	  term MUL factor  { $$ = $1 * $3; }
	| term DIV factor  { $$ = $1 / $3; }
	| term power       { $$ = $1 * $2; }
	| factor           { $$ = $1; }
	;

factor:
	  ADD factor  { $$ = $2; }
	| SUB factor  { $$ = -$2; }
	| power       { $$ = $1; }
	;

power:
	  primary POW exponent  { $$ = pow($1, $3); }
	| primary               { $$ = $1; }
	;

exponent:
	  INTEGER POW exponent  { $$ = pow($1, $3); }
	| INTEGER               { $$ = $1; }
	;

primary:
	  INTEGER                   { $$ = $1; }
	| FLOAT                     { $$ = $1; }
	/* | PVAR */
	/* | DOLLAR IDENT */
	| LPAREN expression RPAREN  { $$ = $2; }
	;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "%s at line %d near '%s'\n", s, yylineno, yytext);
}

int main(int argc, char *argv[])
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
