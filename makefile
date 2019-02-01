
HEADERS = token.h lexer.h ast.h parser.h statement.h expression.h
LIBS = token.o lexer.o ast.o parser.o statement.o expression.o
DEPS = linked_list/linked_list.o dynamic_string/dynamic_string.o
FLAGS = -Wall -Wextra -Werror -pedantic -g
MAKE_FLAGS = --no-print-directory

monkey : monkey.c $(LIBS) $(HEADERS)
	@cd linked_list && make linked_list.o $(MAKE_FLAGS) && cd ..
	@cd dynamic_string && make dynamic_string.o $(MAKE_FLAGS) && cd ..
	@echo 'Compiling [monkey.c]'
	@gcc monkey.c -o monkey $(LIBS) $(DEPS) $(FLAGS)

test : test.c $(LIBS) $(HEADERS)
	@cd linked_list && make linked_list.o $(MAKE_FLAGS) && cd ..
	@cd dynamic_string && make dynamic_string.o $(MAKE_FLAGS) && cd ..
	@echo 'Compiling [test.c]'
	@gcc test.c -o test $(LIBS) $(DEPS) $(FLAGS)

token.o : token.c token.h
	@echo 'Compiling [token.c]'
	@gcc token.c -c $(FLAGS)

lexer.o : lexer.c lexer.h token.h
	@echo 'Compiling [lexer.c]'
	@gcc lexer.c -c $(FLAGS)

statement.o : statement.c statement.h
	@echo 'Compiling [statement.c]'
	@gcc statement.c -c $(FLAGS)

expression.o : expression.c expression.h
	@echo 'Compiling [expression.c]'
	@gcc expression.c -c $(FLAGS)

ast.o : ast.c ast.h
	@echo 'Compiling [ast.c]'
	@gcc ast.c -c $(FLAGS)

parser.o : parser.c parser.h
	@echo 'Compiling [parser.c]'
	@gcc parser.c -c $(FLAGS)

clean : 
	@cd linked_list && make clean $(MAKE_FLAGS) && cd ..
	@echo 'Removing test and all object files'
	@rm -f monkey test *.o 

