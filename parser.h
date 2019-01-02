
#ifndef PARSER_H
#define PARSER_H

typedef struct {
	lexer_t *lexer;
	token_t current_token;
	token_t peek_token;
} parser_t;

parser_t *parser_create 		(lexer_t *l);
void	  parser_next_token	 	(parser_t *p);
void 	  parser_parse_program 	(parser_t *p);

#endif /* PARSER_H */

