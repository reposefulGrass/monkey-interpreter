
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string/dynamic_string.h"
#include "expression.h"
#include "token.h"

// ======== IDENTIFIER ========

expr_t *
expr_identifier_create (token_t token) {
    expr_t *expr = (expr_t *) malloc(sizeof(expr_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expr_identifier_create': Failed to allocate 'expr'!\n");
    }

    expr->type = EXPRESSION_IDENTIFIER;
    EXPR_IDENT(expr) = (identifier_t) { 
        .token = token_dup(token),
        .value = strdup(token.literal)
    };

    expr->token_literal = expr_identifier_token_literal;
    expr->string = expr_identifier_string;
    expr->destroy = expr_identifier_destroy;

    return expr;
}


char *
expr_identifier_token_literal (expr_t *expr) {
    identifier_t ident = EXPR_IDENT(expr);
    return ident.token.literal;
}


char *
expr_identifier_string (expr_t *expr) {
    return strdup(EXPR_IDENT(expr).value);
}


void
expr_identifier_destroy (expr_t *expr) {
    identifier_t ident = EXPR_IDENT(expr);

    token_destroy(&ident.token);
    free(ident.value);
    free(expr);
}

// ======== NUMBER ========

expr_t *  
expr_number_create (token_t token, int value) {
    expr_t *expr = (expr_t *) malloc(sizeof(expr_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expr_number_create': Failed to allocate 'expr'!\n");
    }

    expr->type = EXPRESSION_NUMBER;
    EXPR_NUMBER(expr) = (number_t) { 
        .token = token_dup(token),
        .value = value
    };

    expr->token_literal = expr_number_token_literal;
    expr->string = expr_number_string;
    expr->destroy = expr_number_destroy;

    return expr;
}


char *          
expr_number_token_literal (expr_t *expr) {
    number_t number = EXPR_NUMBER(expr);
    return number.token.literal;
}


char *          
expr_number_string (expr_t *expr) {
    number_t number = EXPR_NUMBER(expr);
    return strdup(number.token.literal);
}


void            
expr_number_destroy (expr_t *expr) {
    number_t number = EXPR_NUMBER(expr);

    token_destroy(&number.token);
    free(expr);
}

// ======== BOOLEAN ========

expr_t *    
expr_boolean_create (token_t token, bool value) {
    expr_t *expr = (expr_t *) malloc(sizeof(expr_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expr_boolean_create': Failed to allocate 'expr'!\n");
    }

    expr->type = EXPRESSION_BOOLEAN;
    EXPR_BOOLEAN(expr) = (boolean_t) { 
        .token = token_dup(token),
        .value = value
    };

    expr->token_literal = expr_boolean_token_literal;
    expr->string = expr_boolean_string;
    expr->destroy = expr_boolean_destroy;

    return expr;
}

char *      
expr_boolean_token_literal (expr_t *expr) {
    boolean_t boolean = EXPR_BOOLEAN(expr);
    return boolean.token.literal;
}

char *      
expr_boolean_string (expr_t *expr) {
    boolean_t boolean = EXPR_BOOLEAN(expr);
    return strdup(boolean.token.literal);
}

void        
expr_boolean_destroy (expr_t *expr) {
    boolean_t boolean = EXPR_BOOLEAN(expr);

    token_destroy(&boolean.token);
    free(expr);
} 


// ======== PREFIX ========

expr_t *  
expr_prefix_create (token_t token, char *op, expr_t *value) {
    expr_t *expr = (expr_t *) malloc(sizeof(expr_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expr_prefix_create': Failed to allocate 'expr'!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPRESSION_PREFIX;
    EXPR_PREFIX(expr) = (prefix_t) {
        .token = token_dup(token),
        .operator = strdup(op),
        .expr = value 
    };

    expr->token_literal = expr_prefix_token_literal;
    expr->string = expr_prefix_string;
    expr->destroy = expr_prefix_destroy;

    return expr;
}


char *  
expr_prefix_token_literal (expr_t *expr) {
    prefix_t prefix = EXPR_PREFIX(expr);
    return prefix.token.literal;
}


char *          
expr_prefix_string (expr_t *expr) {
    prefix_t prefix = EXPR_PREFIX(expr);
    dstr_t *dstring = ds_initialize();

    char *value = STRING(prefix.expr);

    ds_append(dstring, "(");
    ds_append(dstring, prefix.operator);
    ds_append(dstring, value);
    ds_append(dstring, ")");

    char *string = ds_to_string(&dstring);
    free(value);
    return string;
}


void            
expr_prefix_destroy (expr_t *expr) {
    prefix_t prefix = EXPR_PREFIX(expr);

    token_destroy(&prefix.token);
    free(prefix.operator);
    DESTROY(prefix.expr);
    free(expr);
}

// ======== INFIX ========

expr_t *  
expr_infix_create (token_t token, char *op, expr_t *left_expr, expr_t *right_expr) {
    expr_t *expr = (expr_t *) malloc(sizeof(expr_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expr_prefix_create': Failed to allocate 'expr'!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPRESSION_INFIX;
    EXPR_INFIX(expr) = (infix_t) {
        .token = token_dup(token),
        .operator = strdup(op),
        .left_expr = left_expr,
        .right_expr = right_expr
    };

    expr->token_literal = expr_infix_token_literal;
    expr->string = expr_infix_string;
    expr->destroy = expr_infix_destroy;

    return expr;
}


char *  
expr_infix_token_literal (expr_t *expr) {
    infix_t infix = EXPR_INFIX(expr);
    return infix.token.literal;
}


char *          
expr_infix_string (expr_t *expr) {
    infix_t infix = EXPR_INFIX(expr);
    dstr_t *dstring = ds_initialize();

    char *left = STRING(infix.left_expr);
    char *right = STRING(infix.right_expr);

    ds_append(dstring, "(");
    ds_append(dstring, left);
    ds_append(dstring, " ");
    ds_append(dstring, infix.operator);
    ds_append(dstring, " ");
    ds_append(dstring, right);
    ds_append(dstring, ")");

    free(left);
    free(right);

    char *string = ds_to_string(&dstring);
    return string;
}


void            
expr_infix_destroy (expr_t *expr) {
    infix_t infix = EXPR_INFIX(expr);

    token_destroy(&infix.token);
    free(infix.operator);
    DESTROY(infix.left_expr);
    DESTROY(infix.right_expr);
    free(expr);
}

// ======== IF ========

expr_t *    
expr_if_create (token_t token, expr_t *cond, stmt_t *cons, stmt_t *alt) {
    expr_t *expr = (expr_t *) malloc(sizeof(expr_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expr_if_create': Failed to allocate 'expr'!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPRESSION_IF;
    EXPR_IF(expr) = (if_t) {
        .token = token,
        .condition = cond,
        .consequence = cons,
        .alternative = alt
    };

    expr->token_literal = expr_if_token_literal;
    expr->string = expr_if_string;
    expr->destroy = expr_if_destroy;

    return expr;
}


char *      
expr_if_token_literal (expr_t *expr) {
    if_t expr_if = EXPR_IF(expr);
    return expr_if.token.literal;
}


char *      
expr_if_string (expr_t *expr) {
    if_t expr_if = EXPR_IF(expr);
    dstr_t *dstring = ds_initialize();

    ds_append(dstring, "if");
    ds_append(dstring, STRING(expr_if.condition));
    ds_append(dstring, " ");
    ds_append(dstring, STRING(expr_if.consequence));

    if (expr_if.alternative != NULL) {
        ds_append(dstring, "else");
        ds_append(dstring, STRING(expr_if.alternative));
    }

    char *string = ds_to_string(&dstring);
    return string;
}


void
expr_if_destroy (expr_t *expr) {
    if_t expr_if = EXPR_IF(expr);

    token_destroy(&expr_if.token);
    DESTROY(expr_if.condition);
    DESTROY(expr_if.consequence);
    if (expr_if.alternative) DESTROY(expr_if.alternative);
}


expr_t *
expr_fn_defn_create (token_t token, list parameters, stmt_t *body) {
    expr_t *expr = (expr_t *) malloc(sizeof(expr_t));
    if (expr == NULL) {
        fprintf(stderr, "ERROR in 'expr_defn_create': Failed to allocate 'expr'!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPRESSION_FN_DEFN;
    EXPR_FN_DEFN(expr) = (expr_fn_defn_t) {
        .token = token,
        .parameters = parameters,
        .body = body 
    };

    expr->token_literal = expr_fn_defn_token_literal;
    expr->string = expr_fn_defn_string;
    expr->destroy = expr_fn_defn_destroy;
}


char *
expr_fn_defn_token_literal (expr_t *expr) {
    fn_defn_t expr_fn_defn = EXPR_FN_DEFN(expr);
    return expr_fn_defn.token.literal;
}


char *      
expr_fn_defn_string (expr_t *expr) {
    fn_defn_t expr_fn_defn = EXPR_FN_DEFN(expr);
    dstr_t *dstring = ds_initialize();

    ds_append(dstring, "fn (");

    bool first_occur = true;
    list cursor = NULL;
    while ((cursor = ll_iterator(expr_fn_defn.parameters, cursor)) != NULL) {
        if (!first_occur) {
            ds_append(dstring, ", ");
        }

        expr_t *parameter = (expr_t *) cursor->data;
        char *str = STRING(parameter);
        ds_append(dstring, str); 

        first_occur = false;
    }
    ds_append(dstring, ") ");

    ds_append(dstring, STRING(expr_fn_defn.body));

    char *string = ds_to_string(&dstring);
    return string;
}


void    
expr_fn_defn_destroy (expr_t *expr) {
    fn_defn_t expr_fn_defn = EXPR_FN_DEFN(expr);

    token_destroy(&expr_fn_defn.token);
    ll_destroy(&expr_fn_defn.parameters, free);
    DESTROY(expr_fn_defn.body);
}




