
#include <stdio.h>  // for NULL
#include <stdlib.h>

#include "statement.h"
#include "token.h"

statement_t *
statement_let_create (token_t token, expression_t *identifier, expression_t *value) {
    statement_t *stmt = (statement_t *) malloc(sizeof(statement_t));

    stmt->type = STATEMENT_LET;
    stmt->statement.let = (statement_let_t) {
        .token = token,
        .name = identifier,
        .value = value 
    };

    stmt->token_literal = statement_let_token_literal;
    stmt->destroy = statement_let_destroy;

    return stmt;
}

char *
statement_let_token_literal (statement_t *stmt) {
	if (stmt->type != STATEMENT_LET) {
		printf("ERROR: Statement Type is not STATEMENT_LET!\n");
		exit(EXIT_FAILURE);
	}

	statement_let_t let_stmt = stmt->statement.let;	

	return let_stmt.token.literal;
}

void
statement_let_destroy (statement_t *stmt) {
	statement_let_t let_stmt = stmt->statement.let;

	token_destroy(&let_stmt.token);
	let_stmt.name->destroy(let_stmt.name);
	//let_stmt.value->destroy(let_stmt.value);
	free(stmt);
}


statement_t *
statement_return_create (token_t token, expression_t *value) {
    statement_t *stmt = (statement_t *) malloc(sizeof(statement_t));

    stmt->type = STATEMENT_RETURN;
    stmt->statement.ret = (statement_return_t) {
        .token = token,
        .value = value
    };

    stmt->token_literal = statement_return_token_literal;
    stmt->destroy = statement_return_destroy;

    return stmt;
}


char *
statement_return_token_literal (statement_t *stmt) {
    statement_return_t return_stmt = stmt->statement.ret;

    return return_stmt.token.literal;
}


void
statement_return_destroy (statement_t *stmt) {
    statement_return_t return_stmt = stmt->statement.ret;

    token_destroy(&return_stmt.token);
    //return_stmt.value->destroy(return_stmt.value);
    free(stmt);
}


void
statement_destroy (void *data) {
	statement_t *stmt = (statement_t *) data;
	stmt->destroy(stmt);	
}


