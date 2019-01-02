
HEADERS = token.h lexer.h
LIBS = token.o lexer.o
FLAGS = -Wall -g

monkey : monkey.c $(LIBS) $(HEADERS)
	gcc monkey.c -o monkey $(LIBS) $(FLAGS)

test : test.c $(LIBS) $(HEADERS)
	gcc test.c -o test $(LIBS) $(FLAGS)

token.o : token.c token.h
	gcc token.c -c

lexer.o : lexer.c lexer.h token.h
	gcc lexer.c -c

clean : 
	rm test token.o lexer.o

