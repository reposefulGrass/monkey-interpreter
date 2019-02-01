
#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "statement.h"

#define MAX_ERROR_SIZE 200

typedef struct parser 
    parser_t;

struct parser {
	lexer_t *lexer;

	token_t current_token;
	token_t peek_token;

	list errors;            // A list of error encountered during parsing
};

typedef enum {
    PRECEDENCE_LOWEST = 1,
    PRECEDENCE_EQUALS,
    PRECEDENCE_LESSGREATER,
    PRECEDENCE_SUM,
    PRECEDENCE_PRODUCT,
    PRECEDENCE_PREFIX,
    PRECEDENCE_CALL,
} precedence_t;

parser_t *      parser_create 				        (lexer_t *l);
void	        parser_destroy				        (parser_t *p);
void	  	    parser_next_token	 		        (parser_t *p);

program_t *     parser_parse_program 		        (parser_t *p);

statement_t *   parser_parse_statement 	            (parser_t *p);
statement_t *   parser_parse_statement_let          (parser_t *p);
statement_t *   parser_parse_statement_return       (parser_t *p);
statement_t *   parser_parse_statement_expression   (parser_t *p);

expression_t *  parser_parse_expression             (parser_t *p, precedence_t prec);
expression_t *  parser_parse_expression_identifier  (parser_t *p);
expression_t *  parser_parse_expression_number      (parser_t *p);
expression_t *  parser_parse_expression_prefix      (parser_t *p);

bool		    parser_current_token_is	            (parser_t *p, tokentype_t t);
bool		    parser_peek_token_is 		        (parser_t *p, tokentype_t t);
bool		    parser_expect_peek			        (parser_t *p, tokentype_t t);

void            parser_invalid_number_error         (parser_t *parser);
void            parser_no_prefix_fn_error           (parser_t *parser);
void 		    parser_peek_error 			        (parser_t *p, tokentype_t type);
bool		    parser_check_errors		            (parser_t *p);

typedef expression_t * (*fn_ptr)(); // fn_ptr is pointer to a function

fn_ptr          parser_get_prefix_fn            (tokentype_t type);
fn_ptr          parser_get_infix_fn             (tokentype_t type);


#endif /* PARSER_H */

