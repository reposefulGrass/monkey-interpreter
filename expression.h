
#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "token.h"

typedef enum {
	EXPRESSION_IDENTIFIER
} expression_type_t;

typedef struct {
	token_t token;
    char *value;
} identifier_t;

typedef struct expression_t {
	expression_type_t type;
	union {
		identifier_t identifier; 
	} expression;

	char *  (*token_literal)    (struct expression_t *expr);
	void    (*destroy)          (struct expression_t *expr);
} expression_t;

expression_t *  expression_identifier_create        (token_t token);
char *          expression_identifier_token_literal (expression_t *expr);
void            expression_identifier_destroy	    (expression_t *expr);


#endif /* EXPRESSSION_H */
