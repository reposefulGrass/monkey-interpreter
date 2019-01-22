
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "statement.h"
#include "parser.h"


parser_t *
parser_create (lexer_t *l) {
	parser_t *p = (parser_t *) malloc(sizeof(parser_t));
	if (p == NULL) {
		printf("Failed to initilize parser!");	
		exit(EXIT_FAILURE);
	}	

	p->lexer = l;
	// TODO: change to TOKEN_NULL?
	p->current_token = (token_t) {.type = TOKEN_EOF, .literal = ""};
	p->peek_token = (token_t) {.type = TOKEN_EOF, .literal = ""};
	ll_initialize(&p->errors);

	// initialize 'current_token' & 'peek_token' to the first two tokens
	parser_next_token(p);
	parser_next_token(p);

	return p;
}

void
parser_destroy (parser_t *p) {
	CHECK_PARSER_NULL(p);

	ll_destroy(&p->errors, free);
	lexer_destroy(p->lexer);
	free(p);
}

void 
parser_next_token (parser_t *p) {
	CHECK_PARSER_NULL(p);

	token_destroy(&p->current_token);

	p->current_token = p->peek_token;
	p->peek_token = lexer_next_token(p->lexer);
}

program_t *
parser_parse_program (parser_t *p) {
	CHECK_PARSER_NULL(p);

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
	CHECK_PARSER_NULL(p);

	switch (p->current_token.type) {
		case TOKEN_LET:
			return parser_parse_statement_let(p);

		case TOKEN_RETURN:
            return parser_parse_statement_return(p);

		default:
			return NULL;
	}
} 


statement_t *
parser_parse_statement_let (parser_t *p) {
	CHECK_PARSER_NULL(p);

    token_t let_token = token_dup(p->current_token);
    token_t ident_token = token_dup(p->peek_token);

    if (!parser_expect_peek(p, TOKEN_IDENT) || !parser_expect_peek(p, TOKEN_ASSIGN)) {
        return NULL;
    }

    expression_t *identifier = expression_identifier_create(ident_token); 

    // temporarily skip value
	while (!parser_current_token_is(p, TOKEN_SEMICOLON)) {
		parser_next_token(p);
	}

    expression_t *value = NULL;

    return statement_let_create(let_token, identifier, value);
}


statement_t *
parser_parse_statement_return (parser_t *p) {
    CHECK_PARSER_NULL(p);

    token_t return_token = token_dup(p->current_token);    

    // temporarily skip value
    while (!parser_current_token_is(p, TOKEN_SEMICOLON)) {
        parser_next_token(p);
    }

    expression_t *value = NULL;

    return statement_return_create(return_token, value);
}


bool
parser_current_token_is (parser_t *p, tokentype_t type) {
	CHECK_PARSER_NULL(p);	
	return p->current_token.type == type;
}


bool
parser_peek_token_is (parser_t *p, tokentype_t type) {
	CHECK_PARSER_NULL(p);	
	return p->peek_token.type == type;
}


bool
parser_expect_peek (parser_t *p, tokentype_t type) {
	CHECK_PARSER_NULL(p);	

	if (parser_peek_token_is(p, type)) {
		parser_next_token(p);		
		return true;
	}

	parser_peek_error(p, type);
	return false;
}

void
parser_peek_error (parser_t *p, tokentype_t type) {
	CHECK_PARSER_NULL(p);	

	char *curr_type = token_name(type);
	char *peek_type = token_name(p->peek_token.type);

	char *error_msg = 
		"Error at " YELLOW "|%d:%d| " RESET 
		"Expected token to be " GREEN "'%s'" RESET ", got " RED "'%s'" RESET " instead.";

	int error_msg_len = 
		strlen(curr_type) +
		strlen(peek_type) + 
		strlen(error_msg) - (4 * 2) +	// 2 * strlen("%d") + 2 * strlen("%s")
		2 * 10 + 1;						// 2 * size of a maximum integer + NULL

	char *error = (char *) malloc(sizeof(char) * error_msg_len);

	snprintf(
		error,
		error_msg_len,
		error_msg,
		p->peek_token.line,
		p->peek_token.position,
		curr_type,
		peek_type
	);

	ll_append(&p->errors, error);
}


bool 
parser_check_errors	(parser_t *p) {
	CHECK_PARSER_NULL(p);	

	int errors = ll_length(p->errors);
	if (errors == 0) {
		return false;	
	}

	printf("Your program has " RED "'%d' errors!" RESET "\n\n", errors);

	list cursor = NULL;
	while ((cursor = ll_iterator(p->errors, cursor)) != NULL) {
		char *error_msg = (char *) cursor->data;
		printf("%s\n", error_msg);
	}
	putchar('\n');

	return true;
}


