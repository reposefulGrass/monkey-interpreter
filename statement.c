
#include <stdio.h>  // for NULL
#include <stdlib.h>
#include <string.h>

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
        .token = token,
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

    char *literal = stmt->token_literal(stmt);
    char *identifier = let_stmt.name->string(let_stmt.name);
    char *value = NULL;

    // temporary
    if (let_stmt.value != NULL) {
        value = let_stmt.value->string(let_stmt.value);
    }

    int length = 
        strlen(literal) + 1 + // "let "
        strlen(identifier) + 3 + // "identifier = " 
        strlen(value) + 1 + 1; // "value;" & NULL

    char *buffer = (char *) malloc(sizeof(char) * length);
    if (buffer == NULL) {
        printf("ERROR in 'statement_let_string': Failed to initialize 'buffer'!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(buffer, literal); // null-terminate buffer for strcat
    strcat(buffer, " ");
    strcat(buffer, identifier); 
    strcat(buffer, " = ");
    strcat(buffer, value);
    strcat(buffer, ";");

    free(identifier);
    free(value);
    return buffer;
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
        .token = token,
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

    char *literal = stmt->string(stmt);
    char *value = NULL;

    if (return_stmt.value != NULL) {
        value = return_stmt.value->string(return_stmt.value);
    }

    int length = 
        strlen(literal) + 1 + // "return "
        strlen(value) + 2; // "value;" & NULL

    char *buffer = (char *) malloc(sizeof(char) * length + 1);
    if (buffer == NULL) {
        printf("ERROR in 'statement_return_string': Failed to initialize 'buffer'!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(buffer, literal); // null-terminate buffer for strcat
    strcat(buffer, " ");
    strcat(buffer, value);
    strcat(buffer, ";");

    return buffer;
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
        .token = token,
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
    //expr_stmt.value->destroy(expr_stmt.value);
    free(stmt);
}


void
statement_destroy (void *data) {
	statement_t *stmt = (statement_t *) data;
	stmt->destroy(stmt);	
}


