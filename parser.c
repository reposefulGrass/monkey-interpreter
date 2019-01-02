
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
	token_destroy(p->current_token);

	p->current_token = p->peek_token;
	p->peek_token = lexer_next_token(p->lexer);
}

void /* program_t */
parser_parse_program (parser_t *p) {
	return;
}




