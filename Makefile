all: nipel

nipel: lexer.o parser.o
	cc -o nipel lexer.o parser.o

lexer.o: lexer.c parser.h
	cc -c lexer.c

parser.o: parser.c
	cc -c parser.c

lexer.c: lexer.l parser.h
	flex -o lexer.c lexer.l

parser.c parser.h: parser.y
	bison -d -o parser.c parser.y

clean:
	rm -f nipel lexer.c parser.c parser.h *.o

.PHONY: all clean
