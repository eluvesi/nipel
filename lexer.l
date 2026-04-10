%{
#define PVAR     1
#define IDENT    2
#define INTEGER  3
#define FLOAT    4
#define STRING   5
#define NEWLINE  6
#define ASSIGN   7
#define ADD      8
#define SUB      9
#define MUL     10
#define DIV     11
#define POW     12
#define LPAREN  13
#define RPAREN  14
#define DOLLAR  15
%}

%option yylineno noyywrap

%%

[[:lower:]] {
		printf("PVAR(%s)\n", yytext);
		return PVAR;
	}

[[:upper:]][[:upper:][:digit:]_]* {
		printf("IDENT(%s)\n", yytext);
		return IDENT;
	}

\"[^\n"]*\" {
		printf("STRING(%s)\n", strndup(yytext+1, yyleng-2));
		return STRING;
	}

[[:digit:]]+\.[[:digit:]]+ {
		printf("FLOAT(%s)\n", yytext);
		return FLOAT;
	}

[[:digit:]]+ {
		printf("INTEGER(%s)\n", yytext);
		return INTEGER;
	}

"="  { puts("ASSIGN");  return ASSIGN;  }
"+"  { puts("ADD");     return ADD;     }
"-"  { puts("SUB");     return SUB;     }
"*"  { puts("MUL");     return MUL;     }
"/"  { puts("DIV");     return DIV;     }
"^"  { puts("POW");     return POW;     }
"("  { puts("LPAREN");  return LPAREN;  }
")"  { puts("RPAREN");  return RPAREN;  }
"$"  { puts("DOLLAR");  return DOLLAR;  }
\n   { puts("NEWLINE"); return NEWLINE; }

\#.*            /* eat up one-line comments */
[[:blank:]]+    /* eat up blank symbols */

.	{
		fprintf(stderr,
			"error at line %d: unrecognized symbol \'%s\'\n",
			yylineno,
			yytext
		);
		exit(1);
	}

%%

int main(void) 
{
	int tok;
	while ((tok = yylex()) != 0) {
		/* lexer-only mode */
	}
	return 0;
}
