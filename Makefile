CC = gcc
LEX = flex
YACC = bison

CFLAGS = -Wall -Wextra -Wpedantic -std=gnu99
YFLAGS = -d

TARGET = nipel

OBJ = main.o lexer.o parser.o ast.o eval.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

main.o: main.c
	$(CC) $(CFLAGS) -c $<

lexer.o: lexer.c parser.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.c ast.h
	$(CC) $(CFLAGS) -c $<

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c $<

eval.o: eval.c eval.h ast.h
	$(CC) $(CFLAGS) -c $<

lexer.c: lexer.l parser.h
	$(LEX) -o $@ $<

parser.c parser.h: parser.y
	$(YACC) $(YFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ) lexer.c parser.c parser.h 

.PHONY: all clean
