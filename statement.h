
#ifndef STATEMENT_H
#define STATEMENT_H


#include "expression.h"
#include "token.h"

typedef enum {
	STATEMENT_LET,
    STATEMENT_RETURN,
    STATEMENT_EXPRESSION
} statement_type_t;

typedef struct {
	token_t token; 
	expression_t *name; 
	expression_t *value;
} statement_let_t; 

typedef struct {
    token_t token;
    expression_t *value;
} statement_return_t;

typedef struct {
    token_t token;
    expression_t *value;
} statement_expression_t;

typedef struct statement_t {
	statement_type_t type;	
	union {
		statement_let_t let; 
        statement_return_t ret;
        statement_expression_t expr;
	} statement;

	char *  (*token_literal)    (struct statement_t *stmt);
    char *  (*string)           (struct statement_t *stmt);
	void    (*destroy)          (struct statement_t *stmt);
} statement_t; 

statement_t *   statement_let_create                (token_t token, expression_t *identifier, expression_t *value);
char *          statement_let_token_literal 	    (statement_t *stmt);
char *          statement_let_string                (statement_t *stmt);
void            statement_let_destroy 			    (statement_t *stmt);

statement_t *   statement_return_create             (token_t token, expression_t *value);
char *          statement_return_token_literal 	    (statement_t *stmt);
char *          statement_return_string             (statement_t *stmt);
void            statement_return_destroy 		    (statement_t *stmt);

statement_t *   statement_expression_create         (token_t token, expression_t *value);
char *          statement_expression_token_literal  (statement_t *stmt);
char *          statement_expression_string         (statement_t *stmt);
void            statement_expression_destroy 	    (statement_t *stmt);

void            statement_destroy                   (void *data);


#endif /* STATEMENT_H */
