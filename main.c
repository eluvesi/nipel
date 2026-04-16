#include <stdio.h>

extern FILE *yyin;
extern int yyparse(void);


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
