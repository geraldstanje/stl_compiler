CC = g++
YACC = yacc
LEX = lex
CFLAGS = -g -Wall -std=c++11
DFLAGS = -DDEBUG
YACCFLAGS = -dv
LECFLAGS = -l
INCDIR = ./include
INCDIR_EXT = ./ext
SRCDIR = src
SRCDIR_EXT = ext
GENDIR = generated
LIBS = -ll -ly 
SRC = util.cpp tinyxml2.cpp bool_property.cpp node.cpp ast.cpp code_generator.cpp y.tab.cpp lex.yy.cpp stl_comp.cpp
OBJ = util.o tinyxml2.o bool_property.o node.o ast.o code_generator.o y.tab.o lex.yy.o stl_comp.o
TARGET = stl_comp 

all: $(TARGET) $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $(OBJ) $(LIBS)

y.tab.cpp: $(INCDIR)/stl_parser.y
	$(YACC) $(YACCFLAGS) -o y.tab.cpp $(INCDIR)/stl_parser.y

lex.yy.cpp: $(INCDIR)/stl_parser.l
	$(LEX) $(LECFLAGS) -o lex.yy.cpp $(INCDIR)/stl_parser.l

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(DFLAGS) -I$(INCDIR) -I$(INCDIR_EXT) -c $<

%.o: $(SRCDIR_EXT)/%.cpp
	$(CC) $(CFLAGS) $(DFLAGS) -I$(INCDIR_EXT) -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) $(DFLAGS) -I$(INCDIR) -c $<

clean:
	rm -f y.tab.* lex.yy.* y.output plot.dot $(GENDIR)/generated_stl.h $(OBJ) $(TARGET)

