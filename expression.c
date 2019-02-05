
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string/dynamic_string.h"
#include "expression.h"
#include "token.h"

expression_t *
expression_identifier_create (token_t token) {
	expression_t *expr = (expression_t *) malloc(sizeof(expression_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expression_identifier_create': Failed to allocate 'expr'!\n");
    }

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
    expr = NULL;
}

expression_t *  
expression_number_create (token_t token, int value) {
    expression_t *expr = (expression_t *) malloc(sizeof(expression_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expression_number_create': Failed to allocate 'expr'!\n");
    }

	expr->type = EXPRESSION_NUMBER;
	expr->expression.number = (number_t) { 
        .token = token_dup(token),
        .value = value
    };

	expr->token_literal = expression_number_token_literal;
    expr->string = expression_number_string;
	expr->destroy = expression_number_destroy;

	return expr;

}

char *          
expression_number_token_literal (expression_t *expr) {
    number_t number = expr->expression.number;

    return number.token.literal;
}

char *          
expression_number_string (expression_t *expr) {
    number_t number = expr->expression.number;
    
    return strdup(number.token.literal);
}

void            
expression_number_destroy (expression_t *expr) {
    number_t number = expr->expression.number;

    token_destroy(&number.token);
    free(expr);
    expr = NULL; // TODO: Change to 'expression_t **expr' so this works?
}

expression_t *  
expression_prefix_create (token_t token, char *op, expression_t *value) {
    expression_t *expr = (expression_t *) malloc(sizeof(expression_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expression_prefix_create': Failed to allocate 'expr'!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPRESSION_PREFIX;
    expr->expression.prefix = (prefix_t) {
        .token = token_dup(token),
        .operator = strdup(op),
        .expr = value 
    };

    expr->token_literal = expression_prefix_token_literal;
    expr->string = expression_prefix_string;
    expr->destroy = expression_prefix_destroy;

    return expr;
}

char *  
expression_prefix_token_literal (expression_t *expr) {
    prefix_t prefix = expr->expression.prefix;
    return prefix.token.literal;
}

char *          
expression_prefix_string (expression_t *expr) {
    prefix_t prefix = expr->expression.prefix;
    ds_t *dstring = ds_initialize();

    char *value = prefix.expr->string(prefix.expr);

    ds_append(dstring, "(");
    ds_append(dstring, prefix.operator);
    ds_append(dstring, value);
    ds_append(dstring, ")");

    char *string = ds_to_string(dstring);
    free(value);
    return string;
}

void            
expression_prefix_destroy (expression_t *expr) {
    prefix_t prefix = expr->expression.prefix;

    token_destroy(&prefix.token);
    free(prefix.operator);
    prefix.expr->destroy(prefix.expr);
    free(expr);
}

expression_t *  
expression_infix_create (token_t token, char *op, expression_t *left_expr, expression_t *right_expr) {
    expression_t *expr = (expression_t *) malloc(sizeof(expression_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expression_prefix_create': Failed to allocate 'expr'!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPRESSION_INFIX;
    expr->expression.infix = (infix_t) {
        .token = token_dup(token),
        .operator = strdup(op),
        .left_expr = left_expr,
        .right_expr = right_expr
    };

    expr->token_literal = expression_infix_token_literal;
    expr->string = expression_infix_string;
    expr->destroy = expression_infix_destroy;

    return expr;
}

char *  
expression_infix_token_literal (expression_t *expr) {
    infix_t infix = expr->expression.infix;
    return infix.token.literal;
}

char *          
expression_infix_string (expression_t *expr) {
    infix_t infix = expr->expression.infix;
    ds_t *dstring = ds_initialize();

    char *left = infix.left_expr->string(infix.left_expr);
    char *right = infix.right_expr->string(infix.right_expr);

    ds_append(dstring, "(");
    ds_append(dstring, left);
    ds_append(dstring, infix.operator);
    ds_append(dstring, right);
    ds_append(dstring, ")");

    char *string = ds_to_string(dstring);
    free(left);
    free(right);
    return string;
}

void            
expression_infix_destroy (expression_t *expr) {
    infix_t infix = expr->expression.infix;

    token_destroy(&infix.token);
    free(infix.operator);
    infix.left_expr->destroy(infix.left_expr);
    infix.right_expr->destroy(infix.right_expr);
    free(expr);
}


void
expression_destroy (expression_t *expr) {
    expr->destroy(expr); 
}

