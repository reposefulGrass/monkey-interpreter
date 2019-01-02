
#ifndef AST_H
#define AST_H

/* The identifier type is special, because we treat it
 * as an expression but its not simply for convience.
*/
typedef struct {
	token_t token;
	char *value;
} identifier_t; /* expr_ident_t? */


typedef enum {
	STATEMENT_LET
} statement_type_t;

typedef struct {
	statement_type_t type;	
	void *statement;
} statement_t;


typedef enum {
	EXPRESSION_IDENTIFIER
} expression_type_t;

typedef struct {
	expression_type_t type;
	void *expression;
} expression_t;


typedef struct {
	token_t token;
	identifier_t name;
	expression_t value;
} statement_let_t;


typedef struct {
	statement_t *statements;
} program_t;


// FUNCTION HEADERS HERE


#endif /* AST_H */
