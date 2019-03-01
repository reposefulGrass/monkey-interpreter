
#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "token.h"

typedef struct expression
    expr_t;

typedef enum {
	EXPRESSION_IDENTIFIER,
    EXPRESSION_NUMBER,
    EXPRESSION_PREFIX,
    EXPRESSION_INFIX
} expr_type_t;

typedef struct {
	token_t token;
    char *value;
} identifier_t;

typedef struct {
    token_t token;
    int value;
} number_t;

typedef struct {
    token_t token; // operator of the prefix expr
    char *operator;
    expr_t *expr;
} prefix_t;

typedef struct {
    token_t token; // operator of the infix expr
    char *operator;
    expr_t *left_expr;
    expr_t *right_expr;
} infix_t;

//     MACROS FOR ACCESSING THE EXPR UNION 
// ---------------------------------------------
// WARNING: 'e' must be of type 'expr_t *'
#define EXPR_IDENT(e)   ((e)->expr.identifier)
#define EXPR_NUMBER(e)  ((e)->expr.number)
#define EXPR_PREFIX(e)  ((e)->expr.prefix)
#define EXPR_INFIX(e)   ((e)->expr.infix)

//     MACROS FOR CALLING EXPR METHODS
// --------------------------------------------
// WARNING: 'e' must be of type 'stmt_t *' or 'expr_t *'
#ifndef METHOD_MACROS
#define METHOD_MACROS
    #define TOKEN_LITERAL(e) ((e)->token_literal(e))
    #define STRING(e)       ((e)->string(e))
    #define DESTROY(e)      ((e)->destroy(e));
#endif

struct expression {
	expr_type_t type;
	union {
		identifier_t identifier; 
        number_t number;
        prefix_t prefix;
        infix_t infix;
	} expr;

    // ==== METHODS ====
    // this fn will always return a non-allocated literal of the token of the expr
	char *  (*token_literal)    (expr_t *expr);
    // this fn will always return a heap-allocated string
    char *  (*string)           (expr_t *expr);
	void    (*destroy)          (expr_t *expr);
};

expr_t *    expr_identifier_create          (token_t token);
char *      expr_identifier_token_literal   (expr_t *expr);
char *      expr_identifier_string          (expr_t *expr);
void        expr_identifier_destroy	        (expr_t *expr);

expr_t *    expr_number_create              (token_t token, int value);
char *      expr_number_token_literal       (expr_t *expr);
char *      expr_number_string              (expr_t *expr);
void        expr_number_destroy	            (expr_t *expr);

expr_t *    expr_prefix_create              (token_t token, char *op, expr_t *expr);
char *      expr_prefix_token_literal       (expr_t *expr);
char *      expr_prefix_string              (expr_t *expr);
void        expr_prefix_destroy	            (expr_t *expr);

expr_t *    expr_infix_create               (token_t token, char *op, expr_t *lexpr, expr_t *rexpr);
char *      expr_infix_token_literal        (expr_t *expr);
char *      expr_infix_string               (expr_t *expr);
void        expr_infix_destroy              (expr_t *expr);

void        expr_destroy                    (expr_t *expr);


#endif /* EXPRESSSION_H */
