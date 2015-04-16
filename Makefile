CC = g++
YACC = yacc
LEX = lex
CFLAGS = -g -Wall -std=c++11
LDFLAGS = -ll
DFLAGS = -DDEBUG
YACCFLAGS = -dv
#LECFLAGS = -l
#LIBDIR = "C:\Program Files\GnuWin32\lib"
#LIBS = -lfl -ly
SRC = node.cpp code_generator.cpp y.tab.cpp lex.yy.cpp main.cpp
OBJ = node.o code_generator.o y.tab.o lex.yy.o main.o
TARGET = main

all: $(TARGET) $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(DFLAGS) $(LDFLAGS) -o $@ $(OBJ) 
	#-L$(LIBDIR) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(DFLAGS) -c $<

y.tab.cpp: stl_parser.y
	$(YACC) -dv -o y.tab.cpp stl_parser.y

lex.yy.cpp: stl_parser.l
	$(LEX) -l -o lex.yy.cpp stl_parser.l

clean:
	rm -f y.tab.* lex.yy.* y.output $(OBJ) $(TARGET)

