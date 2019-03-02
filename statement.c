
#include <stdio.h>  // for NULL
#include <stdlib.h>
#include <string.h>

#include "dynamic_string/dynamic_string.h"
#include "statement.h"
#include "token.h"


// ======== LET ========

stmt_t *
stmt_let_create (token_t token, expr_t *identifier, expr_t *value) {
    stmt_t *stmt = (stmt_t *) malloc(sizeof(stmt_t));
    if (stmt == NULL) {
        fprintf(stderr, "ERROR in 'stmt_let_create': Failed to allocate 'stmt'!\n");
        exit(EXIT_FAILURE);
    }

    stmt->type = STATEMENT_LET;
    STMT_LET(stmt) = (stmt_let_t) {
        .token = token_dup(token),
        .name = identifier,
        .value = value 
    };

    stmt->token_literal = stmt_let_token_literal;
    stmt->string = stmt_let_string;
    stmt->destroy = stmt_let_destroy;

    return stmt;
}


char *
stmt_let_token_literal (stmt_t *stmt) {
	stmt_let_t let_stmt = STMT_LET(stmt);
	return let_stmt.token.literal;
}


char *
stmt_let_string (stmt_t *stmt) {
    stmt_let_t let_stmt = STMT_LET(stmt);
    dstr_t *dstring = ds_initialize();

    char *literal = TOKEN_LITERAL(stmt);
    char *identifier = STRING(let_stmt.name);

    ds_append(dstring, literal);
    ds_append(dstring, " ");
    ds_append(dstring, identifier);
    ds_append(dstring, " = ");
    free(identifier);

    if (let_stmt.value != NULL) {
        char *value = STRING(let_stmt.value);
        ds_append(dstring, value);
        free(value);
    }
    ds_append(dstring, ";");

    char *ret_str = ds_to_string(&dstring);
    return ret_str;
}


void
stmt_let_destroy (stmt_t *stmt) {
	stmt_let_t let_stmt = STMT_LET(stmt);

	token_destroy(&let_stmt.token);
    // temporary!
    if (let_stmt.value != NULL) {
        DESTROY(let_stmt.value);
    }
	free(stmt);
}

// ======== RETURN ========

stmt_t *
stmt_return_create (token_t token, expr_t *value) {
    stmt_t *stmt = (stmt_t *) malloc(sizeof(stmt_t));
    if (stmt == NULL) {
        fprintf(
            stderr, 
            "ERROR in 'stmt_return_create': Failed to allocate 'stmt'!\n"
        );
        exit(EXIT_FAILURE);
    }

    stmt->type = STATEMENT_RETURN;
    STMT_RETURN(stmt) = (stmt_return_t) {
        .token = token_dup(token),
        .value = value
    };

    stmt->token_literal = stmt_return_token_literal;
    stmt->string = stmt_return_string;
    stmt->destroy = stmt_return_destroy;

    return stmt;
}


char *
stmt_return_token_literal (stmt_t *stmt) {
    stmt_return_t return_stmt = STMT_RETURN(stmt);
    return return_stmt.token.literal;
}


char *
stmt_return_string (stmt_t *stmt) {
    stmt_return_t return_stmt = STMT_RETURN(stmt);
    dstr_t *dstring = ds_initialize();

    char *literal = STRING(stmt);
    ds_append(dstring, literal);
    ds_append(dstring, " ");

    if (return_stmt.value != NULL) {
        char *value = STRING(return_stmt.value);
        ds_append(dstring, value);    
    }
    ds_append(dstring, ";");

    char *string = ds_to_string(&dstring);
    return string;
}


void
stmt_return_destroy (stmt_t *stmt) {
    stmt_return_t return_stmt = STMT_RETURN(stmt);

    token_destroy(&return_stmt.token);
    //DESTROY(return_stmt.value)
    free(stmt);
}

// ======== EXPR ========

stmt_t *   
stmt_expr_create (token_t token, expr_t *value) {
    stmt_t *stmt = (stmt_t *) malloc(sizeof(stmt_t));
    if (stmt == NULL) {
        fprintf(
            stderr, 
            "ERROR in 'stmt_expr_create': Failed to allocate 'stmt'!\n"
        );
        exit(EXIT_FAILURE);
    }

    stmt->type = STATEMENT_EXPRESSION;
    STMT_EXPR(stmt) = (stmt_expr_t) {
        .token = token_dup(token),
        .expr = value
    };

    stmt->token_literal = stmt_expr_token_literal;
    stmt->string = stmt_expr_string;
    stmt->destroy = stmt_expr_destroy;

    return stmt;
}


char *          
stmt_expr_token_literal (stmt_t *stmt) {
    stmt_expr_t expr_stmt = STMT_EXPR(stmt);
    return expr_stmt.token.literal;
}


char *
stmt_expr_string (stmt_t *stmt) {
    stmt_expr_t expr_stmt = STMT_EXPR(stmt);

    // temporary since we havent implemented exprs yet
    if (expr_stmt.expr != NULL) {
        expr_t *expr = expr_stmt.expr;
        return STRING(expr);
    }

    return NULL;
}


void            
stmt_expr_destroy (stmt_t *stmt) {
    stmt_expr_t expr_stmt = STMT_EXPR(stmt);
    token_destroy(&expr_stmt.token);
    DESTROY(expr_stmt.expr);
    free(stmt);
}


void
stmt_destroy (void *data) {
	stmt_t *stmt = (stmt_t *) data;
	stmt->destroy(stmt);	
}


