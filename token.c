
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

char *token_names[] = {
	/* TOKEN_ILLEGAL 	*/ 	"<illegal>",
	/* TOKEN_EOF 		*/	"<eof>",
	/* TOKEN_IDENT 		*/	"<ident>",
	/* TOKEN_NUMBER 	*/	"<number>",
	/* TOKEN_ASSIGN 	*/	"=",
	/* TOKEN_PLUS 		*/	"+",
	/* TOKEN_MINIS		*/ 	"-",
	/* TOKEN_BANG 		*/  "!",
	/* TOKEN_ASTERISK 	*/	"*",
	/* TOKEN_SLASH		*/ 	"/",
	/* TOKEN_EQ			*/  "==",
	/* TOKEN_NEQ		*/	"!=",
	/* TOKEN_LT			*/ 	"<",
	/* TOKEN_GT			*/	">",
	/* TOKEN_COMMA 		*/	",",
	/* TOKEN_SEMICOLON 	*/ 	";",
	/* TOKEN_LPAREN 	*/	"(",
	/* TOKEN_RPAREN 	*/	")",
	/* TOKEN_LBRACE 	*/	"{",
	/* TOKEN_RBRACE 	*/	"}",
	/* TOKEN_FUNCTION 	*/	"fn",
	/* TOKEN_LET 		*/	"let",
	/* TOKEN_TRUE		*/	"true",
	/* TOKEN_FALSE		*/	"false",
	/* TOKEN_IF			*/	"if",
	/* TOKEN_ELSE		*/ 	"else",
	/* TOKEN_RETURN		*/	"return"
};

char *
token_name (tokentype_t token) {
	return token_names[token];
}

// destroy with 'token_destroy()'
token_t
token_create (tokentype_t type, char *ch, unsigned int pos, unsigned int line) {
	token_t token = {
		.type = type,
		.literal = strdup(ch),
		.position = pos,
		.line = line
	};

	return token;
}

void
token_destroy (token_t *token) {
	if (token->type != TOKEN_EOF) {
		free(token->literal);	
	}
	token->literal = NULL;
}

token_t		
token_dup (token_t token) {
	token_t t;
	t.type = token.type;
	t.literal = strdup(token.literal);
	t.position = token.position;
    t.line = token.line;

	return t;
}

tokentype_t
token_lookup_ident (char *identifier) {
	if (strcmp(identifier, token_name(TOKEN_FUNCTION)) == 0) {
		return TOKEN_FUNCTION;
	}
	else if (strcmp(identifier, token_name(TOKEN_LET)) == 0) {
		return TOKEN_LET;
	}
	else if (strcmp(identifier, token_name(TOKEN_TRUE)) == 0) {
		return TOKEN_TRUE;
	}
	else if (strcmp(identifier, token_name(TOKEN_FALSE)) == 0) {
		return TOKEN_FALSE;
	}
	else if (strcmp(identifier, token_name(TOKEN_IF)) == 0) {
		return TOKEN_IF;
	}
	else if (strcmp(identifier, token_name(TOKEN_ELSE)) == 0) {
		return TOKEN_ELSE;
	}
	else if (strcmp(identifier, token_name(TOKEN_RETURN)) == 0) {
		return TOKEN_RETURN;
	}

	return TOKEN_IDENT;
}




