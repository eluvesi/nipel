#include <stdio.h>
#include <unistd.h>


extern FILE *yyin;
extern int yyparse(void);

int is_interactive = 0;  /* flag for interactive mode */


int main(int argc, char *argv[])
{
	/* setup input */
	if (argc < 2 || (argv[1][0] == '-' && argv[1][1] == '\0')) {
		yyin = stdin;
		is_interactive = isatty(fileno(stdin));  /* set interactive flag */
	} else {
		yyin = fopen(argv[1], "r");
		if (!yyin) {
			perror(argv[1]);
			return 1;
		}
	}

	/* in interactive mode greeting and first '> ' prompt needed */
	if (is_interactive)
		printf("Nipel interactive mode. End input with Ctrl+D.\n> ");

	/* parse input */
	yyparse();

	/* in interactive mode one more '\n' needed */
	if(is_interactive)
		printf("\n");

	/* close input file */
	if (yyin != stdin)
		fclose(yyin);

	return 0;
}
