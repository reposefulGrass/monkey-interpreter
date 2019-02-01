
#include <stdio.h>  // for NULL
#include <stdlib.h>
#include <string.h>

#include "dynamic_string/dynamic_string.h"
#include "statement.h"
#include "token.h"

statement_t *
statement_let_create (token_t token, expression_t *identifier, expression_t *value) {
    statement_t *stmt = (statement_t *) malloc(sizeof(statement_t));
    if (stmt == NULL) {
        fprintf(stderr, "ERROR in 'statement_let_create': Failed to allocate 'stmt'!\n");
        exit(EXIT_FAILURE);
    }

    stmt->type = STATEMENT_LET;
    stmt->statement.let = (statement_let_t) {
        .token = token_dup(token),
        .name = identifier,
        .value = value 
    };

    stmt->token_literal = statement_let_token_literal;
    stmt->string = statement_let_string;
    stmt->destroy = statement_let_destroy;

    return stmt;
}

char *
statement_let_token_literal (statement_t *stmt) {
	statement_let_t let_stmt = stmt->statement.let;	
	return let_stmt.token.literal;
}


char *
statement_let_string (statement_t *stmt) {
    statement_let_t let_stmt = stmt->statement.let;
    ds_t *dstring = ds_initialize();

    char *literal = stmt->token_literal(stmt);
    char *identifier = let_stmt.name->string(let_stmt.name);
    ds_append(dstring, literal);
    ds_append(dstring, " ");
    ds_append(dstring, identifier);
    ds_append(dstring, " = ");
    free(identifier);

    // temporary
    if (let_stmt.value != NULL) {
        char *value = let_stmt.value->string(let_stmt.value);
        ds_append(dstring, value);
        free(value);
    }
    ds_append(dstring, ";");

    char *ret_str = ds_to_string(dstring);
    return ret_str;
}

void
statement_let_destroy (statement_t *stmt) {
	statement_let_t let_stmt = stmt->statement.let;

	token_destroy(&let_stmt.token);
	let_stmt.name->destroy(let_stmt.name);
    // temporary!
    if (let_stmt.value != NULL) {
        let_stmt.value->destroy(let_stmt.value);
    }
	free(stmt);
}


statement_t *
statement_return_create (token_t token, expression_t *value) {
    statement_t *stmt = (statement_t *) malloc(sizeof(statement_t));
    if (stmt == NULL) {
        fprintf(stderr, "ERROR in 'statement_return_create': Failed to allocate 'stmt'!\n");
        exit(EXIT_FAILURE);
    }

    stmt->type = STATEMENT_RETURN;
    stmt->statement.ret = (statement_return_t) {
        .token = token_dup(token),
        .value = value
    };

    stmt->token_literal = statement_return_token_literal;
    stmt->string = statement_return_string;
    stmt->destroy = statement_return_destroy;

    return stmt;
}


char *
statement_return_token_literal (statement_t *stmt) {
    statement_return_t return_stmt = stmt->statement.ret;
    return return_stmt.token.literal;
}


char *
statement_return_string (statement_t *stmt) {
    statement_return_t return_stmt = stmt->statement.ret;
    ds_t *dstring = ds_initialize();

    char *literal = stmt->string(stmt);
    ds_append(dstring, literal);

    if (return_stmt.value != NULL) {
        char *value = return_stmt.value->string(return_stmt.value);
        ds_append(dstring, value);    
    }
    ds_append(dstring, ";");

    char *string = ds_to_string(dstring);
    return string;
}


void
statement_return_destroy (statement_t *stmt) {
    statement_return_t return_stmt = stmt->statement.ret;

    token_destroy(&return_stmt.token);
    //return_stmt.value->destroy(return_stmt.value);
    free(stmt);
}


statement_t *   
statement_expression_create (token_t token, expression_t *value) {
    statement_t *stmt = (statement_t *) malloc(sizeof(statement_t));
    if (stmt == NULL) {
        fprintf(stderr, "ERROR in 'statement_expression_create': Failed to allocate 'stmt'!\n");
        exit(EXIT_FAILURE);
    }

    stmt->type = STATEMENT_EXPRESSION;
    stmt->statement.expr = (statement_expression_t) {
        .token = token_dup(token),
        .expr = value
    };

    stmt->token_literal = statement_expression_token_literal;
    stmt->string = statement_expression_string;
    stmt->destroy = statement_expression_destroy;

    return stmt;
}


char *          
statement_expression_token_literal (statement_t *stmt) {
    statement_expression_t expr_stmt = stmt->statement.expr;
    return expr_stmt.token.literal;
}


char *
statement_expression_string (statement_t *stmt) {
    statement_expression_t expr_stmt = stmt->statement.expr;

    // temporary since we havent implemented expressions yet
    if (expr_stmt.expr != NULL) {
        expression_t *expr = expr_stmt.expr;
        return strdup(expr->string(expr));
    }

    return NULL;
}


void            
statement_expression_destroy (statement_t *stmt) {
    statement_expression_t expr_stmt = stmt->statement.expr;
    token_destroy(&expr_stmt.token);
    expression_destroy(expr_stmt.expr);
    free(stmt);
}


void
statement_destroy (void *data) {
	statement_t *stmt = (statement_t *) data;
	stmt->destroy(stmt);	
}


