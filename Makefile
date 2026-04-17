CC = gcc
LEX = flex
YACC = bison

CFLAGS = -Wall -Wextra -Wpedantic -std=gnu99
YFLAGS = -d

TARGET = nipel

OBJ = main.o lexer.o parser.o ast.o eval.o env.o poly.o

all: $(TARGET)


# target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm


# object files
main.o: main.c
	$(CC) $(CFLAGS) -c $<

lexer.o: lexer.c parser.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.c ast.h eval.h poly.h
	$(CC) $(CFLAGS) -c $<

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c $<

eval.o: eval.c eval.h ast.h env.h poly.o
	$(CC) $(CFLAGS) -c $<

env.o: env.c env.h poly.h
	$(CC) $(CFLAGS) -c $<

poly.o: poly.c poly.h
	$(CC) $(CFLAGS) -c $<


# yacc/lex
lexer.c: lexer.l parser.h
	$(LEX) -o $@ $<

parser.c parser.h: parser.y
	$(YACC) $(YFLAGS) $<


# clean
clean:
	rm -f $(TARGET) $(OBJ) lexer.c parser.c parser.h 

.PHONY: all clean
