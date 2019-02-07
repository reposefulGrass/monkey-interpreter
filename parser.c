
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "colors.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "statement.h"
#include "parser.h"


parser_t *
parser_create (lexer_t *l) {
	parser_t *p = (parser_t *) malloc(sizeof(parser_t));
	if (p == NULL) {
		fprintf(stderr, "ERROR in 'parser_create': Failed to allocate 'parser'!");	
		exit(EXIT_FAILURE);
	}	

	p->lexer = l;
	p->current_token = (token_t) {.type = TOKEN_EOF, .literal = ""};
	p->peek_token = (token_t) {.type = TOKEN_EOF, .literal = ""};
	ll_initialize(&p->errors);

	// initialize 'current_token' & 'peek_token' to the first two tokens
	parser_next_token(p);
	parser_next_token(p);

	return p;
}

void
parser_destroy (parser_t *p) {
	ll_destroy(&p->errors, free);
	lexer_destroy(p->lexer);
	free(p);
}

void 
parser_next_token (parser_t *p) {
	token_destroy(&p->current_token);

	p->current_token = p->peek_token;
	p->peek_token = lexer_next_token(p->lexer);
}

program_t *
parser_parse_program (parser_t *p) {
	program_t *program = ast_program_create();

	while (p->current_token.type != TOKEN_EOF) {
		statement_t *stmt = parser_parse_statement(p);
		if (stmt != NULL) {
			ll_append(&program->statements, (void *) stmt);
		}
		parser_next_token(p);
	}	

	return program;
}

statement_t *
parser_parse_statement (parser_t *p) {
	switch (p->current_token.type) {
		case TOKEN_LET:
			return parser_parse_statement_let(p);

		case TOKEN_RETURN:
            return parser_parse_statement_return(p);

		default:
            return parser_parse_statement_expression(p);
	}
} 


statement_t *
parser_parse_statement_let (parser_t *p) {
    token_t let_token = token_dup(p->current_token);
    token_t ident_token = token_dup(p->peek_token);

    if (!parser_expect_peek(p, TOKEN_IDENT) || !parser_expect_peek(p, TOKEN_ASSIGN)) {
        return NULL;
    }

    expression_t *identifier = expression_identifier_create(ident_token); 

    // temporarily skip value
	while (!parser_current_token_is(p, TOKEN_SEMICOLON)) {
		parser_next_token(p);
	}

    expression_t *value = NULL;

    statement_t *let_stmt = statement_let_create(let_token, identifier, value);
    token_destroy(&let_token);
    token_destroy(&ident_token);

    return let_stmt;
}


statement_t *
parser_parse_statement_return (parser_t *p) {
    token_t return_token = token_dup(p->current_token);    

    // temporarily skip value
    while (!parser_current_token_is(p, TOKEN_SEMICOLON)) {
        parser_next_token(p);
    }

    expression_t *value = NULL;

    statement_t *ret_stmt = statement_return_create(return_token, value);
    token_destroy(&return_token);

    return ret_stmt;
}


statement_t *
parser_parse_statement_expression (parser_t *parser) {
    token_t expr_token = token_dup(parser->current_token);
    expression_t *expr = parser_parse_expression(parser, PRECEDENCE_LOWEST);

    if (expr == NULL) {
        token_destroy(&expr_token);
        return NULL;
    }

    if (parser_peek_token_is(parser, TOKEN_SEMICOLON)) {
        parser_next_token(parser); 
    }

    statement_t *expr_stmt = statement_expression_create(expr_token, expr);
    token_destroy(&expr_token);

    return expr_stmt;
}

expression_t *  
parser_parse_expression (parser_t *parser, precedence_t precedence) {
    fn_ptr prefix = parser_get_prefix_fn(parser->current_token.type);     
    if (prefix == NULL) {
        parser_no_prefix_fn_error(parser);
        return NULL; 
    }
    expression_t *left_expr = prefix(parser);

    while (
        !parser_peek_token_is(parser, TOKEN_SEMICOLON) && 
        precedence < parser_peek_precedence(parser)
    ) {
        fn_ptr infix = parser_get_infix_fn(parser->peek_token.type);
        if (infix == NULL) {
            return left_expr;
        }
        
        parser_next_token(parser); 
        left_expr = infix(parser, left_expr);
    }

    return left_expr;
}

precedence_t    
parser_get_precedence (tokentype_t type) {
    switch (type) {
        case TOKEN_EQ:          return PRECEDENCE_EQUALS;
        case TOKEN_NEQ:         return PRECEDENCE_EQUALS;
        case TOKEN_LT:          return PRECEDENCE_LESSGREATER;
        case TOKEN_GT:          return PRECEDENCE_LESSGREATER;
        case TOKEN_PLUS:        return PRECEDENCE_SUM;
        case TOKEN_MINUS:       return PRECEDENCE_SUM;
        case TOKEN_SLASH:       return PRECEDENCE_PRODUCT;
        case TOKEN_ASTERISK:    return PRECEDENCE_PRODUCT;
        default:                return PRECEDENCE_LOWEST;
    }

    return PRECEDENCE_LOWEST;
}

fn_ptr          
parser_get_prefix_fn (tokentype_t type) {
    switch (type) {
        case TOKEN_IDENT:
            return parser_parse_expression_identifier;

        case TOKEN_NUMBER: 
            return parser_parse_expression_number;

        case TOKEN_BANG:
        case TOKEN_MINUS:
            return parser_parse_expression_prefix;

        default:
            return NULL;
    } 
}

fn_ptr
parser_get_infix_fn (tokentype_t type) {
    switch (type) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_SLASH:
        case TOKEN_ASTERISK:
        case TOKEN_EQ:
        case TOKEN_NEQ:
        case TOKEN_LT:
        case TOKEN_GT:
            return parser_parse_expression_infix;

        default:
            return NULL;
    }
}


expression_t *
parser_parse_expression_identifier (parser_t *parser) {
    return expression_identifier_create(parser->current_token);
}

expression_t *  
parser_parse_expression_number (parser_t *parser) {
    token_t curr_token = parser->current_token;

    long number = strtol(curr_token.literal, NULL, 10);
    if (errno == ERANGE || number < INT_MIN || number > INT_MAX) {
        parser_invalid_number_error(parser);
        return NULL;
    }

    return expression_number_create(curr_token, number);
}


expression_t *
parser_parse_expression_prefix (parser_t *parser) {
    token_t curr_token = token_dup(parser->current_token);
    char *operator = curr_token.literal;

    parser_next_token(parser);
    expression_t *right = parser_parse_expression(parser, PRECEDENCE_PREFIX);

    expression_t *prefix_expr =  expression_prefix_create(curr_token, operator, right);
    token_destroy(&curr_token);
    return prefix_expr;
}


expression_t *
parser_parse_expression_infix (parser_t *parser, expression_t *left_expr) {
    token_t curr_token = token_dup(parser->current_token);
    char *operator = curr_token.literal;

    precedence_t prec = parser_curr_precedence(parser);
    parser_next_token(parser);
    expression_t *right_expr = parser_parse_expression(parser, prec);

    expression_t *infix_expr = expression_infix_create(curr_token, operator, left_expr, right_expr);
    token_destroy(&curr_token);
    return infix_expr;
}


precedence_t
parser_curr_precedence (parser_t *parser) {
    return parser_get_precedence(parser->current_token.type);
}


precedence_t
parser_peek_precedence (parser_t *parser) {
    return parser_get_precedence(parser->peek_token.type);
}


bool
parser_current_token_is (parser_t *p, tokentype_t type) {
	return p->current_token.type == type;
}


bool
parser_peek_token_is (parser_t *p, tokentype_t type) {
	return p->peek_token.type == type;
}


bool
parser_expect_peek (parser_t *p, tokentype_t type) {
	if (parser_peek_token_is(p, type)) {
		parser_next_token(p);		
		return true;
	}

	parser_peek_error(p, type);
	return false;
}


void
parser_invalid_number_error (parser_t *parser) {
    // create error message
    char *error_msg = 
	    "Error at " YELLOW "|%d:%d| " RESET "Expected a number in the range of %d to %d!\n";

    int error_msg_len = 
	    strlen(error_msg) - (4 * 2) +	// 4 * strlen("%d")
	    4 * 10 + 1;						// 4 * size of a maximum integer + NULL

    char *error = (char *) malloc(sizeof(char) * error_msg_len);
    if (error == NULL) {
        fprintf(stderr, "ERROR in 'parser_invalid_number_error': Failed to allocate 'error'!\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
        error, error_msg_len, error_msg, 
        parser->current_token.line, parser->current_token.position, INT_MIN, INT_MAX
    );
    ll_append(&parser->errors, error);
}


void
parser_no_prefix_fn_error (parser_t *parser) {
    char *typename = token_name(parser->current_token.type);

    char *error_msg = 
        "Error at " YELLOW "|%d:%d| " RESET
        "No prefix parse function for '%s' found!";

    int error_msg_len =
        strlen(typename) + 
        strlen(error_msg) - (3 * 2) +   // 2 * strlen("%d") + strlen("%s")
        2 * 10 + 1;                     // 2 * size of a maximum integer + NULL

    char *error = (char *) malloc(sizeof(char) * error_msg_len);
    if (error == NULL) {
        fprintf(
            stderr, 
            "ERROR in 'parser_no_prefix_fn_error': Failed to allocate 'error'!\n"
        );
        exit(EXIT_FAILURE);
    }

    snprintf(
        error, error_msg_len, error_msg, 
        parser->current_token.line, parser->current_token.position, typename
    ); 

    ll_append(&parser->errors, error);
}


void
parser_peek_error (parser_t *p, tokentype_t type) {
	char *curr_type = token_name(type);
	char *peek_type = token_name(p->peek_token.type);

	char *error_msg = 
		"Error at " YELLOW "|%d:%d| " RESET 
		"Expected token to be " GREEN "'%s'" RESET ", got " RED "'%s'" RESET " instead.";

	int error_msg_len = 
		strlen(curr_type) +
		strlen(peek_type) + 
		strlen(error_msg) - (4 * 2) +	// 2 * strlen("%d") + 2 * strlen("%s")
		2 * 10 + 1;						// 2 * size of a maximum integer + NULL

	char *error = (char *) malloc(sizeof(char) * error_msg_len);
    if (error == NULL) {
        fprintf(stderr, "ERROR in 'parser_peek_error': Failed to allocate 'error'!\n");
        exit(EXIT_FAILURE);
    }

	snprintf(
		error,
		error_msg_len,
		error_msg,

		p->peek_token.line,
		p->peek_token.position,
		curr_type,
		peek_type
	);

	ll_append(&p->errors, error);
}


bool 
parser_check_errors	(parser_t *p) {
	int errors = ll_length(p->errors);
	if (errors == 0) {
		return false;	
	}

	printf("Your program has " RED "'%d' errors!" RESET "\n\n", errors);

	list cursor = NULL;
	while ((cursor = ll_iterator(p->errors, cursor)) != NULL) {
		char *error_msg = (char *) cursor->data;
		printf("%s\n", error_msg);
	}
	putchar('\n');

	return true;
}


