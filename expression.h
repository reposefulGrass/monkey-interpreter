
#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "token.h"

typedef struct expression_t
    expression_t;

typedef enum {
	EXPRESSION_IDENTIFIER,
    EXPRESSION_NUMBER,
    EXPRESSION_PREFIX
} expression_type_t;

typedef struct {
	token_t token;
    char *value;
} identifier_t;

typedef struct {
    token_t token;
    int value;
} number_t;

typedef struct {
    token_t token; // operator of the prefix expr
    char *operator;
    expression_t *expr;
} prefix_t;

struct expression_t {
	expression_type_t type;
	union {
		identifier_t identifier; 
        number_t number;
        prefix_t prefix;
	} expression;

	char *  (*token_literal)    (struct expression_t *expr);
    char *  (*string)           (struct expression_t *expr);
	void    (*destroy)          (struct expression_t *expr);
};

expression_t *  expression_identifier_create        (token_t token);
char *          expression_identifier_token_literal (expression_t *expr);
char *          expression_identifier_string        (expression_t *expr);
void            expression_identifier_destroy	    (expression_t *expr);

expression_t *  expression_number_create            (token_t token, int value);
char *          expression_number_token_literal     (expression_t *expr);
char *          expression_number_string            (expression_t *expr);
void            expression_number_destroy	        (expression_t *expr);

expression_t *  expression_prefix_create            (token_t token, char *op, expression_t *expr);
char *          expression_prefix_token_literal     (expression_t *expr);
char *          expression_prefix_string            (expression_t *expr);
void            expression_prefix_destroy	        (expression_t *expr);

void            expression_destroy                  (expression_t *expr);


#endif /* EXPRESSSION_H */
