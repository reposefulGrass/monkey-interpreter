
#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "statement.h"

#define MAX_ERROR_SIZE 200

typedef struct {
	lexer_t *lexer;
	token_t current_token;
	token_t peek_token;
	list errors;
} parser_t;

parser_t *      parser_create 				    (lexer_t *l);
void	        parser_destroy				    (parser_t *p);
void	  	    parser_next_token	 		    (parser_t *p);
program_t *     parser_parse_program 		    (parser_t *p);
statement_t *   parser_parse_statement 	        (parser_t *p);
statement_t *   parser_parse_statement_let      (parser_t *p);
statement_t *   parser_parse_statement_return   (parser_t *p);
bool		    parser_current_token_is	        (parser_t *p, tokentype_t t);
bool		    parser_peek_token_is 		    (parser_t *p, tokentype_t t);
bool		    parser_expect_peek			    (parser_t *p, tokentype_t t);
void 		    parser_peek_error 			    (parser_t *p, tokentype_t type);
bool		    parser_check_errors		        (parser_t *p);

#endif /* PARSER_H */

