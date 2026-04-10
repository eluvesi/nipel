all: nipel

nipel: lexer.c
	cc -o nipel lexer.c

lexer.c: lexer.l
	flex -o lexer.c lexer.l

clean:
	rm -f nipel lexer.c

.PHONY: all clean
