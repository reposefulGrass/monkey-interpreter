
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "linked_list.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"


#define TRUE  1
#define FALSE 0

void test_token ();
void test_let_statements();

int 
main () {
	//test_token();
	test_let_statements();
}

bool
test_let_statement (statement_t *stmt, char *expected_identifier) {
	if (strcmp(stmt->token_literal(stmt), "let") != 0) {
		printf(
			"ERROR: stmt->token_literal() not 'let'. got '%s'\n", 
			stmt->token_literal(stmt)
		);
		return false;
	}

	if (stmt->type != STATEMENT_LET) {
		printf("ERROR: stmt->type not STATEMENT_LET. got '%d'\n", stmt->type);
		return false;
	}

	statement_let_t *let_stmt = (statement_let_t *) (stmt->stmt);
	
	if (strcmp(let_stmt->name.value, expected_identifier) != 0) {
		printf(
			"ERROR: let_stmt->name.value not '%s'. got '%s'.\n",
			expected_identifier,
			let_stmt->name.value
		);
		return false;
	}

	if (strcmp(let_stmt->name.token.literal, expected_identifier) == 0) {
		printf(
			"ERROR: let_stmt->name.token.literal not '%s'. got '%s'.\n",
			expected_identifier,
			let_stmt->name.token.literal
		);
		return false;
	}

	return true;
}

void
test_let_statements() {
	char *input = "\
let x = 5;\
let y = 10;\
let foobar = 838383;";

	lexer_t *lexer = lexer_create(input);
	parser_t *parser = parser_create(lexer);

	program_t *program = parser_parse_program(parser);

	if (program == NULL) {
		printf("ERROR: Failed to initialize program_t!\n");
		goto free_resources;
	}

	if (ll_length(program->statements) != 3) {
		printf("ERROR: The number of statements is not 3!\n");
		goto free_resources;
	}

	char *tests[] = {
		"x",
		"y",
		"foobar"
	};

	int index = 0;
	list cursor = NULL;
	while ((cursor = ll_iterator(program->statements, cursor)) != NULL) {
		statement_t *stmt = ast_get_stmt(cursor);
		if (!test_let_statement(stmt, tests[index])) {
			goto free_resources;
		}
		
		index++;	
	}

free_resources:
	//free(program);
	//parser_destroy_program(program); /* ll_destroy()? */
	free(parser);
	free(lexer);	
}

void 
test_token () {
	char *input = "\
let five = 5;\
let ten = 10;\
\
let add = fn(x, y) {\
	x + y;\
};\
\
let result = add(five, ten);\
\
!-/*5;\
5 < 10 > 5;\
\
if (5 < 10) {\
	return true;\
} else {\
	return false;\
}\
\
10 == 10;\
10 != 9;\
";

	bool passed = true;

	token_t tests[] = {
		{TOKEN_LET, "let"},
		{TOKEN_IDENT, "five"},
		{TOKEN_ASSIGN, "="},
		{TOKEN_INT, "5"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_LET, "let"},
		{TOKEN_IDENT, "ten"},
		{TOKEN_ASSIGN, "="},
		{TOKEN_INT, "10"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_LET, "let"},
		{TOKEN_IDENT, "add"},
		{TOKEN_ASSIGN, "="},
		{TOKEN_FUNCTION, "fn"},
		{TOKEN_LPAREN, "("},
		{TOKEN_IDENT, "x"},
		{TOKEN_COMMA, ","},
		{TOKEN_IDENT, "y"},
		{TOKEN_RPAREN, ")"},
		{TOKEN_LBRACE, "{"},
		{TOKEN_IDENT, "x"},
		{TOKEN_PLUS, "+"},
		{TOKEN_IDENT, "y"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_RBRACE, "}"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_LET, "let"},
		{TOKEN_IDENT, "result"},
		{TOKEN_ASSIGN, "="},
		{TOKEN_IDENT, "add"},
		{TOKEN_LPAREN, "("},
		{TOKEN_IDENT, "five"},
		{TOKEN_COMMA, ","},
		{TOKEN_IDENT, "ten"},
		{TOKEN_RPAREN, ")"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_BANG, "!"},
		{TOKEN_MINUS, "-"},
		{TOKEN_SLASH, "/"},
		{TOKEN_ASTERISK, "*"},
		{TOKEN_INT, "5"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_INT, "5"},
		{TOKEN_LT, "<"},
		{TOKEN_INT, "10"},
		{TOKEN_GT, ">"},
		{TOKEN_INT, "5"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_IF, "if"},
		{TOKEN_LPAREN, "("},
		{TOKEN_INT, "5"},
		{TOKEN_LT, "<"},
		{TOKEN_INT, "10"},
		{TOKEN_RPAREN, ")"},
		{TOKEN_LBRACE, "{"},
		{TOKEN_RETURN, "return"},
		{TOKEN_TRUE, "true"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_RBRACE, "}"},
		{TOKEN_ELSE, "else"},
		{TOKEN_LBRACE, "{"},
		{TOKEN_RETURN, "return"},
		{TOKEN_FALSE, "false"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_RBRACE, "}"},
		{TOKEN_INT, "10"},
		{TOKEN_EQ, "=="},
		{TOKEN_INT, "10"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_INT, "10"},
		{TOKEN_NEQ, "!="},
		{TOKEN_INT, "9"},
		{TOKEN_SEMICOLON, ";"},
		{TOKEN_EOF, ""},
	};

	lexer_t *lexer = lexer_create(input);

	int length = sizeof(tests) / sizeof(token_t);
	for (int index = 0; index < length; index++) {
		token_t token = lexer_next_token(lexer);
		token_t expected = tests[index];

		if (token.type != expected.type) {
			passed = false;
			printf(
				"tests[%d] | tokentype wrong. expected `%s`, got `%s`\n",
				index,
				token_name(token.type),
				token_name(expected.type)
			);
		}

		if (strcmp(token.literal, expected.literal) != 0) {
			passed = false;
			printf(
				"tests[%d] | literal wrong. expected `%s`, got `%s`\n",
				index,
				token.literal,
				expected.literal
			);
		}

		token_destroy(&token);
	}

	if (passed == false) {
		printf("Test 'token' has failed!\n");
	}
	else {
		printf("Test 'token' has passed!\n");
	}

	free(lexer);
}
