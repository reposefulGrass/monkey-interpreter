
#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"

parser_t *
parser_create (lexer_t *l) {
	parser_t *p = (parser_t *) malloc(sizeof(parser_t));
	if (p == NULL) {
		printf("Failed to initilize parser!");	
		exit(EXIT_FAILURE);
	}	

	p->lexer = l;
	/* NOTE: change to TOKEN_NULL? */
	p->current_token = (token_t) {.type = TOKEN_EOF, .literal = ""};
	p->peek_token = (token_t) {.type = TOKEN_EOF, .literal = ""};

	/* initialize 'current_token' & 'peek_token' */	
	parser_next_token(p);
	parser_next_token(p);

	return p;
}

void 
parser_next_token (parser_t *p) {
	if (p == NULL) {
		printf("Error: Trying to use a nulled pointer 'p'");
		exit(EXIT_FAILURE);
	}

	token_destroy(&p->current_token);

	p->current_token = p->peek_token;
	p->peek_token = lexer_next_token(p->lexer);
}

// return a linked list of statements
program_t *
parser_parse_program (parser_t *p) {
	program_t *program = ast_program_create();

	while (p->current_token.type != TOKEN_EOF) {
		statement_t *stmt = parser_parse_statement(p);
		if (stmt != NULL) {
			ll_append(&program->statements, (void *) stmt);
		}
		parser_next_token(p);
	}	

	return program;
}

statement_t *
parser_parse_statement (parser_t *p) {
	switch (p->current_token.type) {
		case TOKEN_LET:
			return parser_parse_statement_let(p);

		default:
			return NULL;
	}
} 


statement_t *
parser_parse_statement_let (parser_t *p) {
	statement_t *stmt = (statement_t *) malloc(sizeof(statement_t));
	stmt->type = STATEMENT_LET;
	stmt->statement.let.token = token_dup(p->current_token);

	if (!parser_expect_peek(p, TOKEN_IDENT)) {
		return NULL;
	}

	stmt->statement.let.name = ast_identifier_create(p->current_token);
	stmt->token_literal = ast_stmt_let_token_literal;
	stmt->destroy = ast_stmt_let_destroy;

	if (!parser_expect_peek(p, TOKEN_ASSIGN)) {
		return NULL;
	}	

	while (!parser_current_token_is(p, TOKEN_SEMICOLON)) {
		parser_next_token(p);
	}

	return stmt;
}


bool
parser_current_token_is (parser_t *p, tokentype_t type) {
	return p->current_token.type == type;
}


bool
parser_peek_token_is (parser_t *p, tokentype_t type) {
	return p->peek_token.type == type;
}


bool
parser_expect_peek (parser_t *p, tokentype_t type) {
	if (parser_peek_token_is(p, type)) {
		parser_next_token(p);		
		return true;
	}

	return false;
}
