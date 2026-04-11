CC = gcc
LEX = flex
YACC = bison

CFLAGS = -Wall -Wextra -Wpedantic -std=gnu99
YFLAGS = -d

TARGET = nipel

OBJS = lexer.o parser.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

lexer.o: lexer.c parser.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.c
	$(CC) $(CFLAGS) -c $<

lexer.c: lexer.l parser.h
	$(LEX) -o lexer.c $<

parser.c parser.h: parser.y
	$(YACC) $(YFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) lexer.c parser.c parser.h 

.PHONY: all clean
