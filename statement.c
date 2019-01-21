
#include <stdio.h>  // for NULL
#include <stdlib.h>

#include "statement.h"
#include "token.h"

statement_t *
statement_let_create (token_t token, expression_t *identifier, expression_t *value) {
    return NULL;
}

char *
statement_let_token_literal (statement_t *stmt) {
	// replace with function redirecting to subcalls by type?
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

void
statement_destroy (void *data) {
	statement_t *stmt = (statement_t *) data;
	stmt->destroy(stmt);	
}


