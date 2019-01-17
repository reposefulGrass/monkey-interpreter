
#ifndef AST_H
#define AST_H

#include "linked_list/linked_list.h"
#include "token.h"


// ======== EXPRESSIONS ========

typedef enum {
	EXPRESSION_IDENTIFIER
} expression_type_t;

// TODO: Make this an expression? 
typedef struct {
	token_t token;
} identifier_t;

typedef struct expression_t {
	expression_type_t type;
	union {
		identifier_t identifier; 
	} expression;

	char *(*token_literal)(struct expression_t *expr);
	void (*destroy)(struct expression_t *expr);
} expression_t;


expression_t ast_identifier_create (token_t token);

char *ast_expr_ident_token_literal 	(expression_t *expr);
void  ast_expr_ident_destroy		(expression_t *expr);


// ======== STATEMENTS ========

typedef enum {
	STATEMENT_LET
} statement_type_t;

typedef struct {
	token_t token; 
	expression_t name; 
	expression_t value;
} statement_let_t; 

typedef struct statement_t {
	statement_type_t type;	
	union {
		statement_let_t let; 
	} statement;

	char *(*token_literal)(struct statement_t *stmt);
	void (*destroy)(struct statement_t *stmt);
} statement_t; 

char *ast_stmt_let_token_literal 	(statement_t *stmt);
void  ast_stmt_let_destroy 			(statement_t *stmt);

// ======== PROGRAM ========

typedef struct {
	list statements;
} program_t;


program_t *ast_program_create 	();
void	   ast_program_destroy 	(program_t *program);

char *ast_program_token_literal (program_t *p);



#endif /* AST_H */
