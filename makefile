
ODIR = obj
SDIR = src

HEADERS = $(SDIR)/token.h $(SDIR)/lexer.h $(SDIR)/ast.h $(SDIR)/parser.h $(SDIR)/statement.h $(SDIR)/expression.h $(SDIR)/test.h
LIBS = $(ODIR)/token.o $(ODIR)/lexer.o $(ODIR)/ast.o $(ODIR)/parser.o $(ODIR)/statement.o $(ODIR)/expression.o
DEPS = $(SDIR)/linked_list/linked_list.o $(SDIR)/dynamic_string/dynamic_string.o
FLAGS = -Wall -Wextra -Werror -pedantic -g
MAKE_FLAGS = --no-print-directory

monkey : $(SDIR)/monkey.c $(LIBS) $(HEADERS)
	@cd $(SDIR)/linked_list && make linked_list.o $(MAKE_FLAGS) && cd ../..
	@cd $(SDIR)/dynamic_string && make dynamic_string.o $(MAKE_FLAGS) && cd ../..
	@echo 'Compiling [monkey.c]'
	@gcc $(SDIR)/monkey.c -o monkey $(LIBS) $(DEPS) $(FLAGS)

test : $(SDIR)/test.c $(LIBS) $(HEADERS)
	@cd $(SDIR)/linked_list && make linked_list.o $(MAKE_FLAGS) && cd ../..
	@cd $(SDIR)/dynamic_string && make dynamic_string.o $(MAKE_FLAGS) && cd ../..
	@echo 'Compiling [test.c]'
	@gcc $(SDIR)/test.c -o test $(LIBS) $(DEPS) $(FLAGS)

$(ODIR)/token.o : $(SDIR)/token.c $(SDIR)/token.h
	@echo 'Compiling [token.c]'
	@cd $(SDIR) && gcc token.c -c $(FLAGS) -o ../obj/token.o && cd ..

$(ODIR)/lexer.o : $(SDIR)/lexer.c $(SDIR)/lexer.h $(SDIR)/token.h
	@echo 'Compiling [lexer.c]'
	@cd $(SDIR) && gcc lexer.c -c $(FLAGS) -o ../obj/lexer.o && cd ..

$(ODIR)/statement.o : $(SDIR)/statement.c $(SDIR)/statement.h
	@echo 'Compiling [statement.c]'
	@cd $(SDIR) && gcc statement.c -c $(FLAGS) -o ../obj/statement.o && cd ..

$(ODIR)/expression.o : $(SDIR)/expression.c $(SDIR)/expression.h
	@echo 'Compiling [expression.c]'
	@cd $(SDIR) && gcc expression.c -c $(FLAGS) -o ../obj/expression.o && cd ..

$(ODIR)/ast.o : $(SDIR)/ast.c $(SDIR)/ast.h
	@echo 'Compiling [ast.c]'
	@cd $(SDIR) && gcc ast.c -c $(FLAGS) -o ../obj/ast.o && cd ..

$(ODIR)/parser.o : $(SDIR)/parser.c $(SDIR)/parser.h
	@echo 'Compiling [parser.c]'
	@cd $(SDIR) && gcc parser.c -c $(FLAGS) -o ../obj/parser.o && cd ..

clean : 
	@cd $(SDIR)/linked_list && make clean $(MAKE_FLAGS) && cd ../..
	@cd $(SDIR)/dynamic_string && make clean $(MAKE_FLAGS) && cd ../..
	@echo 'Removing test and all object files'
	@rm -f monkey test obj/*.o 

