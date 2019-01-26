
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "token.h"

expression_t *
expression_identifier_create (token_t token) {
	expression_t *expr = (expression_t *) malloc(sizeof(expression_t));

	expr->type = EXPRESSION_IDENTIFIER;
	expr->expression.identifier = (identifier_t) { 
        .token = token_dup(token),
        .value = strdup(token.literal)
    };

	expr->token_literal = expression_identifier_token_literal;
    expr->string = expression_identifier_string;
	expr->destroy = expression_identifier_destroy;

	return expr;
}

char *
expression_identifier_token_literal (expression_t *expr) {
	identifier_t ident = expr->expression.identifier;
	return ident.token.literal;
}

char *
expression_identifier_string (expression_t *expr) {
    return strdup(expr->expression.identifier.value);
}

void
expression_identifier_destroy (expression_t *expr) {
    identifier_t ident = expr->expression.identifier;

    token_destroy(&ident.token);
    free(ident.value);
    free(expr);
}


