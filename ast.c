
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


char *
ast_program_token_literal (program_t *p) {
	if (! ll_is_empty(p->statements)) {
		statement_t *stmt = (statement_t *) p->statements->data;
		return stmt->token_literal(stmt);
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

	statement_let_t let_stmt = stmt->statement.let;	

	return let_stmt.token.literal;
}

void
ast_stmt_let_destroy (statement_t *stmt) {
	token_destroy(&stmt->statement.let.token);
	ast_expr_ident_destroy(&stmt->statement.let.name);		
	free(stmt);
}

char *
ast_expr_ident_token_literal (expression_t *expr) {
	if (expr->type != EXPRESSION_IDENTIFIER) {
		printf("ERROR: Expression Type is not EXPRESSION_IDENTIFIER!\n");
		exit(EXIT_FAILURE);
	}

	identifier_t ident = expr->expression.identifier;

	return ident.token.literal;
}

void
ast_expr_ident_destroy (expression_t *expr) {
	if (expr->type != EXPRESSION_IDENTIFIER) {
		printf("Error: tried to destroy a non-identifier!\n");
		exit(EXIT_FAILURE);
	}

	free(expr->expression.identifier.token.literal);
}

expression_t 
ast_identifier_create (token_t token) {
	expression_t expr;

	expr.type = EXPRESSION_IDENTIFIER;
	expr.expression.identifier = (identifier_t) {token_dup(token)};

	expr.token_literal = ast_expr_ident_token_literal;
	expr.destroy = ast_expr_ident_destroy;

	return expr;
}

void
ast_statement_destroy (void *data) {
	statement_t *stmt = (statement_t *) data;
	stmt->destroy(stmt);	
}

program_t *
ast_program_create () {
	program_t *program = (program_t *) malloc(sizeof(program_t));

	list new;
	ll_initialize(&new);	

	program->statements = new;	

	return program;
}

void
ast_program_destroy (program_t *program) {
	ll_destroy(&program->statements, ast_statement_destroy);
}


