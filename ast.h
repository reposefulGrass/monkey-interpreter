
#ifndef AST_H
#define AST_H

#include "linked_list.h"
#include "token.h"

/* The identifier type is special, because we treat it
 * as an expression but its not simply for convience.
*/
typedef struct {
	token_t token;
	char *value;
} identifier_t; /* expr_ident_t? */


typedef enum {
	STATEMENT_LET
} statement_type_t;

/* Whenever a statement_t is created, you must set 'type' properly 
 * and must set the function 'token_literal', 'destroy' to its 
 * corresponding type.
*/
typedef struct statement_t {
	statement_type_t type;	
	void *stmt;

	char *(*token_literal)(struct statement_t *stmt);
	void (*destroy)(struct statement_t *stmt);
} statement_t; 


typedef enum {
	EXPRESSION_IDENTIFIER
} expression_type_t;

/* Whenever an expression_t is created, you must set 'type' properly
 * and must set the functions 'token_literal', 'destroy' to its
 * corresponding type.
*/
typedef struct expression_t {
	expression_type_t type;
	void *expression;

	char (*token_literal)(struct expression_t *expr);
	void (*destroy)(struct expression_t *expr);
} expression_t;


typedef struct {
	token_t token;
	identifier_t name; /* possibly a pointer? */
	expression_t value;
} statement_let_t;


typedef struct {
	list statements;
} program_t;


statement_t *ast_get_stmt (list stmt);


#endif /* AST_H */
