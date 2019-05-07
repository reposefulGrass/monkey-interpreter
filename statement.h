
#ifndef STATEMENT_H
#define STATEMENT_H

// This declaration is used by expression.h:
// (hence it being in front of the include)
typedef struct stmt_block stmt_block_t;

typedef struct statement
    stmt_t;

#include "linked_list/linked_list.h"
#include "expression.h"
#include "token.h"

typedef struct stmt_let     stmt_let_t;
typedef struct stmt_return  stmt_return_t;
typedef struct stmt_expr    stmt_expr_t;

typedef enum {
    STATEMENT_LET,
    STATEMENT_RETURN,
    STATEMENT_EXPRESSION,
    STATEMENT_BLOCK
} stmt_type_t;

struct stmt_let {
    token_t token; 
    expr_t *name; 
    expr_t *value;
};

struct stmt_return {
    token_t token;
    expr_t *value;
};

struct stmt_expr {
    token_t token;
    expr_t *expr;
};

struct stmt_block {
    token_t token;
    list statements;
};

//     MACROS FOR ACCESSING THE STMT UNION 
// ---------------------------------------------
// WARNING: 's' must be of type 'stmt_t *'
#define STMT_LET(s)     ((s)->stmt.let)
#define STMT_RETURN(s)  ((s)->stmt.ret)
#define STMT_EXPR(s)    ((s)->stmt.expr)
#define STMT_BLOCK(s)   ((s)->stmt.block)

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
        stmt_block_t block;
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
void        stmt_let_destroy            (stmt_t *stmt);

stmt_t *    stmt_return_create          (token_t token, expr_t *value);
char *      stmt_return_token_literal   (stmt_t *stmt);
char *      stmt_return_string          (stmt_t *stmt);
void        stmt_return_destroy         (stmt_t *stmt);

stmt_t *    stmt_expr_create            (token_t token, expr_t *value);
char *      stmt_expr_token_literal     (stmt_t *stmt);
char *      stmt_expr_string            (stmt_t *stmt);
void        stmt_expr_destroy           (stmt_t *stmt);

stmt_t *    stmt_block_create           (token_t token, list statements);
char *      stmt_block_token_literal    (stmt_t *stmt);
char *      stmt_block_string           (stmt_t *stmt);
void        stmt_block_destroy          (stmt_t *stmt);

void        stmt_destroy                (void *data);

#endif /* STATEMENT_H */

