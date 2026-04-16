%{
#include <stdlib.h>
#include "parser.h"
%}

%option yylineno noyywrap noinput nounput

%%

[[:lower:]] {
                yylval.sval = strdup(yytext);
                return PVAR;
            }

[[:upper:]][[:upper:][:digit:]_]* {
                                      yylval.sval = strdup(yytext);
                                      return IDENT;
                                  }

\"[^\n"]*\" {
                yylval.sval = strndup(yytext + 1, yyleng - 2);
                return STRING;
            }

[[:digit:]]+(\.[[:digit:]]+)? {
                                  yylval.dval = atof(yytext);
                                  return NUMBER;
                              }

"="     return ASSIGN;
"+"     return ADD;
"-"     return SUB;
"*"     return MUL;
"/"     return DIV;
"^"     return POW;
"("     return LPAREN;
")"     return RPAREN;
"$"     return DOLLAR;
\n      return NEWLINE;

\#.*            /* eat up one-line comments */
[[:blank:]]+    /* eat up blank symbols */

.	{
		fprintf(stderr,
		        "lexical error at line %d: illegal character '%c' (%d)\n",
		        yylineno, yytext[0], (unsigned char)yytext[0]);
		exit(1);
	}

%%
