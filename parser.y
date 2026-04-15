%{
#include <stdio.h>

extern FILE *yyin;
extern int yylineno;
extern char *yytext;

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
	  expression ADD term
	| expression SUB term
	| term
	;

term:
	  term MUL factor
	| term DIV factor
	| term power
	| factor
	;

factor:
	  ADD power
	| SUB power
	| power
	;

power:
	  primary POW exponent
	| primary
	;

exponent:
	  INTEGER POW exponent
	| INTEGER
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
