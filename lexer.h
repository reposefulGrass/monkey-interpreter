
#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
	char *input;	// the source code to lex
	int length;		// the length of the source code (used in lexer_readchar)
	int pos;		// the position of 'ch' in 'input'
	int read_pos;	// the position of the next char to read
	char ch;		// the current char being looked at
} lexer_t;


lexer_t    *lexer_create 		(char *input);
void		lexer_destroy 		(lexer_t *l);
void 		lexer_read_char 	(lexer_t *l);
token_t		lexer_next_token 	(lexer_t *l);


#endif /* LEXER_H */
