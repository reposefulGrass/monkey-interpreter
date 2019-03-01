
#ifndef STATEMENT_H
#define STATEMENT_H


#include "expression.h"
#include "token.h"

typedef struct statement
    stmt_t;

typedef enum {
	STATEMENT_LET,
    STATEMENT_RETURN,
    STATEMENT_EXPRESSION
} stmt_type_t;

typedef struct {
	token_t token; 
	expr_t *name; 
	expr_t *value;
} stmt_let_t; 

typedef struct {
    token_t token;
    expr_t *value;
} stmt_return_t;

typedef struct {
    token_t token;
    expr_t *expr;
} stmt_expr_t;

//     MACROS FOR ACCESSING THE STMT UNION 
// ---------------------------------------------
// WARNING: 's' must be of type 'stmt_t *'
#define STMT_LET(s)     ((s)->stmt.let)
#define STMT_RETURN(s)  ((s)->stmt.ret)
#define STMT_EXPR(s)    ((s)->stmt.expr)

//     MACROS FOR CALLING STATEMENT METHODS
// --------------------------------------------
// WARNING: 's' must be of type 'stmt_t *' or 'expr_t *'
#ifndef METHOD_MACROS
#define METHOD_MACROS
    #define TOKEN_LITERAL(s) ((s)->token_literal(s))
    #define STRING(s)       ((s)->string(s))
    #define DESTROY(s)      ((s)->destroy(s))
#endif

struct statement {
	stmt_type_t type;	
	union {
		stmt_let_t let; 
        stmt_return_t ret;
        stmt_expr_t expr;
	} stmt;

    // ==== METHODS ====
    // this fn will always return a non-allocated literal of the token of the expr
	char *  (*token_literal)    (stmt_t *stmt);
    // this fn will always return an heap-allocated string
    char *  (*string)           (stmt_t *stmt);
	void    (*destroy)          (stmt_t *stmt);
};

stmt_t *    stmt_let_create             (token_t token, expr_t *identifier, expr_t *value);
char *      stmt_let_token_literal      (stmt_t *stmt);
char *      stmt_let_string             (stmt_t *stmt);
void        stmt_let_destroy 		    (stmt_t *stmt);

stmt_t *    stmt_return_create          (token_t token, expr_t *value);
char *      stmt_return_token_literal 	(stmt_t *stmt);
char *      stmt_return_string          (stmt_t *stmt);
void        stmt_return_destroy 		(stmt_t *stmt);

stmt_t *    stmt_expr_create            (token_t token, expr_t *value);
char *      stmt_expr_token_literal     (stmt_t *stmt);
char *      stmt_expr_string            (stmt_t *stmt);
void        stmt_expr_destroy 	        (stmt_t *stmt);

void        stmt_destroy                (void *data);

#endif /* STATEMENT_H */

