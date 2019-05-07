
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

    // A list of errors encountered during parsing
    list errors;            
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

parser_t *      parser_create                   (lexer_t *l);
void            parser_destroy                  (parser_t *p);
void            parser_next_token               (parser_t *p);

program_t *     parser_parse_program            (parser_t *p);

stmt_t *        parser_parse_stmt               (parser_t *p);
stmt_t *        parser_parse_stmt_let           (parser_t *p);
stmt_t *        parser_parse_stmt_return        (parser_t *p);
stmt_t *        parser_parse_stmt_expr          (parser_t *p);
stmt_t *        parser_parse_stmt_block         (parser_t *p);

expr_t *        parser_parse_expr               (parser_t *p, precedence_t prec);
expr_t *        parser_parse_expr_identifier    (parser_t *p);
expr_t *        parser_parse_expr_number        (parser_t *p);
expr_t *        parser_parse_expr_boolean       (parser_t *p);
expr_t *        parser_parse_expr_prefix        (parser_t *p);
expr_t *        parser_parse_expr_infix         (parser_t *p, expr_t *left);
expr_t *        parser_parse_expr_group         (parser_t *p);
expr_t *        parser_parse_expr_if            (parser_t *p);

bool            parser_current_token_is         (parser_t *p, tokentype_t t);
bool            parser_peek_token_is            (parser_t *p, tokentype_t t);
bool            parser_expect_peek              (parser_t *p, tokentype_t t);

void            parser_invalid_number_error     (parser_t *parser);
void            parser_no_prefix_fn_error       (parser_t *parser);
void            parser_peek_error               (parser_t *p, tokentype_t type);
bool            parser_check_errors             (parser_t *p);

precedence_t    parser_get_precedence           (tokentype_t type);
precedence_t    parser_curr_precedence          (parser_t *parser);
precedence_t    parser_peek_precedence          (parser_t *parser);

// expr_fn_ptr is pointer to a function that has the return-type of 'expr_t *'
typedef expr_t * (*expr_fn_ptr)(); 

expr_fn_ptr     parser_get_prefix_fn            (tokentype_t type);
expr_fn_ptr     parser_get_infix_fn             (tokentype_t type);


#endif /* PARSER_H */

