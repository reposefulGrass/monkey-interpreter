
HEADERS = token.h lexer.h ast.h linked_list.h
LIBS = token.o lexer.o ast.o linked_list.o
FLAGS = -Wall -g

monkey : monkey.c $(LIBS) $(HEADERS)
	gcc monkey.c -o monkey $(LIBS) $(FLAGS)

test : test.c $(LIBS) $(HEADERS)
	gcc test.c -o test $(LIBS) $(FLAGS)

token.o : token.c token.h
	gcc token.c -c $(FLAGS)

lexer.o : lexer.c lexer.h token.h
	gcc lexer.c -c $(FLAGS)

ast.o : ast.c ast.h
	gcc ast.c -c $(FLAGS)

linked_list.o : linked_list.c linked_list.h
	gcc linked_list.c -c $(FLAGS)

clean : 
	rm monkey test *.o

