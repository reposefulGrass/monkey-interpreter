
#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "token.h"

expression_t *
expression_identifier_create (token_t token) {
	expression_t *expr = (expression_t *) malloc(sizeof(expression_t));

	expr->type = EXPRESSION_IDENTIFIER;
	expr->expression.identifier = (identifier_t) { .token = token_dup(token) };

	expr->token_literal = expression_ident_token_literal;
	expr->destroy = expression_ident_destroy;

	return expr;
}

char *
expression_ident_token_literal (expression_t *expr) {
	if (expr->type != EXPRESSION_IDENTIFIER) {
		printf("ERROR: Expression Type is not EXPRESSION_IDENTIFIER!\n");
		exit(EXIT_FAILURE);
	}

	identifier_t ident = expr->expression.identifier;

	return ident.token.literal;
}

void
expression_ident_destroy (expression_t *expr) {
	if (expr->type != EXPRESSION_IDENTIFIER) {
		printf("ERROR: tried to destroy a non-identifier as an identifier!\n");
		exit(EXIT_FAILURE);
	}

    token_destroy(&expr->expression.identifier.token);
    free(expr);
}

