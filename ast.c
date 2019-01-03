
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

statement_t *
ast_get_stmt (list stmt) {
	return (statement_t *) stmt->data;	
}

char *
ast_program_token_literal (program_t *p) {
	if (! ll_is_empty(p->statements)) {
		return ast_get_stmt(p->statements)
			->token_literal(ast_get_stmt(p->statements));
	}
	
	return "";
}

char *
ast_stmt_let_token_literal (statement_t *stmt) {
	// replace with function redirecting to subcalls by type?
	if (stmt->type != STATEMENT_LET) {
		printf("ERROR: Statement Type is not STATEMENT_LET!\n");
		exit(EXIT_FAILURE);
	}

	statement_let_t *let_stmt = (statement_let_t *) (stmt->stmt);	

	return let_stmt->token.literal;
}

char *
ast_expr_ident_token_literal (expression_t *expr) {
	if (expr->type != EXPRESSION_IDENTIFIER) {
		printf("ERROR: Expression Type is not EXPRESSION_IDENTIFIER!\n");
		exit(EXIT_FAILURE);
	}

	identifier_t *ident = (identifier_t *) (expr->expression);

	return ident->value;
}

