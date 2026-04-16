%code {
	#include <stdio.h>
	#include "eval.h"

	extern int yylineno;
	extern char *yytext;

	int yylex(void);
	void yyerror(const char *s)
	{
		fprintf(stderr, "%s at line %d near '%s'\n", s, yylineno, yytext);
	}
}

%code requires {
	#include "ast.h"
}

%defines "parser.h"
%output  "parser.c"

%union {
	long ival;
	double dval;
	char *sval;
	Treeptr node;
}

%token <ival> INTEGER
%token <dval> FLOAT
%token <sval> STRING IDENT PVAR
%token ASSIGN ADD SUB MUL DIV POW
%token LPAREN RPAREN DOLLAR NEWLINE

%type <node> expression term factor power exponent primary

%%

program:
	  /* empty */
	| program statement
	;

statement:
	  assignment NEWLINE
	| expression NEWLINE  { printf("%g\n", eval($1)); }
	| STRING NEWLINE      { puts($1); }
	| NEWLINE
	;

assignment:
	  IDENT ASSIGN expression  { printf("%s = %g\n", $1, eval($3)); }
	;

expression:
	  expression ADD term  { $$ = new_binop(OP_ADD, $1, $3); }
	| expression SUB term  { $$ = new_binop(OP_SUB, $1, $3); }
	| term                 { $$ = $1; }
	;

term:
	  term MUL factor  { $$ = new_binop(OP_MUL, $1, $3); }
	| term DIV factor  { $$ = new_binop(OP_DIV, $1, $3); }
	| term power       { $$ = new_binop(OP_MUL, $1, $2); }
	| factor           { $$ = $1; }
	;

factor:
	  SUB factor  { $$ = new_unop(OP_NEG, $2); }
	| ADD factor  { $$ = $2; }
	| power       { $$ = $1; }
	;

power:
	  primary POW exponent  { $$ = new_binop(OP_POW, $1, $3); }
	| primary               { $$ = $1; }
	;

exponent:
	  INTEGER POW exponent  { $$ = new_binop(OP_POW, new_const($1), $3); }
	| INTEGER               { $$ = new_const($1); }
	;

primary:
	  INTEGER                   { $$ = new_const($1); }
	| FLOAT                     { $$ = new_const($1); }
	/* | PVAR */
	/* | DOLLAR IDENT */
	| LPAREN expression RPAREN  { $$ = $2; }
	;

%%
