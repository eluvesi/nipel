%code {
	#include <stdio.h>
	#include "eval.h"
	#include "poly.h"

	extern int yylineno;
	extern char *yytext;
	extern int is_interactive;

	int yylex(void);
	void yyerror(const char *s)
	{
		fprintf(stderr, "%s at line %d near '%s'\n", s,
		        yytext[0] == '\n' ? yylineno - 1 : yylineno,
		        yytext[0] == '\n' ? "\\n" : yytext);
	}
}

%code requires {
	#include "ast.h"
}

%defines "parser.h"
%output  "parser.c"

%union {
	double dval;
	char *sval;
	char pvar;
	Node node;
}

%token <dval> NUMBER
%token <sval> STRING IDENT
%token <pvar> PVAR
%token ASSIGN ADD SUB MUL DIV POW
%token LPAREN RPAREN DOLLAR NEWLINE

%type <node> expression term factor power primary assignment

%%

program:
	  /* empty */
	| NEWLINE program           /* skip leading newlines */
	| statements
	;

statements:
	  statements NEWLINE statement
	| statements NEWLINE        /* skip trailing newlines */
	| statement
	;

statement:
	  assignment	{
	            		Polynomial p = eval($1);
	            		if (is_interactive) {
	            			poly_print(p);
	            			printf("\n> ");
	            		}
	            		poly_free(p);
	            	}
	| expression	{
	            		Polynomial p = eval($1);
	            		poly_print(p);
	            		printf("\n");
	            		if (is_interactive)
	            			printf("> ");
	            		poly_free(p);
	            	}
	| STRING    	{ printf("%s", $1); }
	;

assignment:
	  IDENT ASSIGN assignment  { $$ = node_assign($1, $3); }
	| IDENT ASSIGN expression  { $$ = node_assign($1, $3); }
	;

expression:
	  expression ADD term  { $$ = node_binop(OP_ADD, $1, $3); }
	| expression SUB term  { $$ = node_binop(OP_SUB, $1, $3); }
	| term                 { $$ = $1; }
	;

term:
	  term MUL factor  { $$ = node_binop(OP_MUL, $1, $3); }
	| term DIV factor  { $$ = node_binop(OP_DIV, $1, $3); }
	| term power       { $$ = node_binop(OP_MUL, $1, $2); }
	| factor           { $$ = $1; }
	;

factor:
	  SUB factor  { $$ = node_unop(OP_NEG, $2); }
	| ADD factor  { $$ = $2; }
	| power       { $$ = $1; }
	;

power:
	  primary POW power  { $$ = node_binop(OP_POW, $1, $3); }
	| primary            { $$ = $1; }
	;

primary:
	  NUMBER                    { $$ = node_num($1); }
	| PVAR                      { $$ = node_pvar($1); }
	| DOLLAR IDENT              { $$ = node_ident($2); }
	| LPAREN expression RPAREN  { $$ = $2; }
	;

%%
