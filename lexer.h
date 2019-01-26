
#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include "token.h"


typedef struct {
	char *input;	// the source code to lex (WARNING: must be a dynamically allocated string)
	int length;		// the length of the source code (used in lexer_readchar)
	int pos;		// the position of 'ch' in 'input'
	int read_pos;	// the position of the next char to read
	char ch;		// the current char being looked at
	int curr_line;	// the line number the lexer is on
	int nth_char; 	// the nth char on the current line
} lexer_t;


lexer_t    *lexer_create 		(char *input);
void		lexer_destroy 		(lexer_t *l);
void 		lexer_read_char 	(lexer_t *l);
token_t		lexer_next_token 	(lexer_t *l);


#endif /* LEXER_H */
