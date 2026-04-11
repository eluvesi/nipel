%{
#include "parser.h"
%}

%option yylineno noyywrap noinput nounput

%%

[[:lower:]]                             return PVAR;
[[:upper:]][[:upper:][:digit:]_]*       return IDENT;
\"[^\n"]*\"                             return STRING;

[[:digit:]]+\.[[:digit:]]+      return FLOAT;
[[:digit:]]+                    return INTEGER;

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
			"Lexical error at line %d: unrecognized symbol \'%s\'\n",
			yylineno,
			yytext
		);
		exit(1);
	}

%%
