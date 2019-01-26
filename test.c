
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "linked_list/linked_list.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"


void test_token ();
void test_let_statements ();
void test_return_statements ();
void test_statement_string_fn ();

int 
main () {
	test_token();
	test_let_statements();
    test_return_statements();
    test_statement_string_fn();
}

// whenever I can parse expression change this hardcode into input!
void
test_statement_string_fn () {
    bool passed = true;

    token_t token = token_create(TOKEN_LET, "let", 0, 0);

    token_t identifier_token = token_create(TOKEN_IDENT, "myVar", 0, 0);
    expression_t *identifier = expression_identifier_create(identifier_token);
    token_destroy(&identifier_token);

    token_t value_token = token_create(TOKEN_IDENT, "anotherVar", 0, 0);
    expression_t *value = expression_identifier_create(value_token);
    token_destroy(&value_token);

    statement_t *stmt = statement_let_create(token, identifier, value);

    program_t *program = ast_program_create();
	ll_append(&program->statements, (void *) stmt);
   
    char *program_str = ast_program_string(program); 
    if (strcmp(program_str, "let myVar = anotherVar;") != 0) {
        passed = false;
        printf("ast_program_string() is wrong. got = '%s'\n", program_str);
    }
    free(program_str);

    ast_program_destroy(program);

    if (passed == true) {
        printf("Test 'string_fn' has passed!\n");
    }
    else {
        printf("Test 'string_fn' has failed!\n");
    }
}

void
test_return_statements() {
	char *input_raw = 
		"return 5;\n"
        "return 10;\n"
        "return 993322;\n";
	char *input = strdup(input_raw);

	bool passed = true;

	lexer_t *lexer = lexer_create(input);
	parser_t *parser = parser_create(lexer);

	program_t *program = parser_parse_program(parser);
	if (parser_check_errors(parser)) {
		passed = false;
		goto free_resources;
	}

	if (program == NULL) {
		passed = false;
		printf("ERROR: Failed to initialize 'program'!\n");
		goto free_resources;
	}

	if (ll_length(program->statements) != 3) {
		passed = false;
		printf("ERROR: The number of statements is not 3!\n");
		goto free_resources;
	}

	list cursor = NULL;
	while ((cursor = ll_iterator(program->statements, cursor)) != NULL) {
		statement_t *stmt = (statement_t *) cursor->data;

        if (stmt->type != STATEMENT_RETURN) {
            passed = false;
            printf(
                "ERROR: stmt is not of type STATEMENT_RETURN; got type ID: '%d'.\n", 
                stmt->type
            );
            continue;
        }

        if (strcmp(stmt->token_literal(stmt), "return") != 0) {
            passed = false;
            printf(
                "stmt->token_literal(stmt) NOT 'return'; got '%s' instead.\n",
                stmt->token_literal(stmt)
            );
        }
	}

free_resources:
	ast_program_destroy(program);
	parser_destroy(parser);

	if (passed == false) {
		printf("Test 'return_statements' has failed!\n");
	}
	else {
		printf("Test 'return_statements' has passed!\n");
	}
}

bool
test_let_statement (statement_t *stmt, char *expected_identifier) {
	if (stmt->type != STATEMENT_LET) {
		printf("ERROR: stmt->type not STATEMENT_LET. got '%d'\n", stmt->type);
		return false;
	}

	if (strcmp(stmt->token_literal(stmt), "let") != 0) {
		printf(
			"ERROR: stmt->token_literal() not 'let'. got '%s'\n", 
			stmt->token_literal(stmt)
		);
		return false;
	}

	statement_let_t let_stmt = stmt->statement.let;

	char *identifier = let_stmt.name->token_literal(let_stmt.name);	
	if (strcmp(identifier, expected_identifier)) {
		printf(
			"ERROR: let_stmt->name.token.literal not '%s'. got '%s'.\n",
			expected_identifier,
			identifier
		);
		return false;
	}

	return true;
}

void
test_let_statements() {
	char *input_raw = 
		"let x = 5;\n"
		"let y = 10;\n"
		"let foobar = 838383;\n";
	char *input = strdup(input_raw);

	bool passed = true;

	lexer_t *lexer = lexer_create(input);
	parser_t *parser = parser_create(lexer);

	program_t *program = parser_parse_program(parser);
	if (parser_check_errors(parser)) {
		passed = false;
		goto free_resources;
	}

	if (program == NULL) {
		passed = false;
		printf("ERROR: Failed to initialize 'program'!\n");
		goto free_resources;
	}

	if (ll_length(program->statements) != 3) {
		passed = false;
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
		statement_t *stmt = (statement_t *) cursor->data;
		if (!test_let_statement(stmt, tests[index])) {
			passed = false;
			goto free_resources;
		}
		
		index++;	
	}

free_resources:
	ast_program_destroy(program);
	parser_destroy(parser);

	if (passed == false) {
		printf("Test 'let_statements' has failed!\n");
	}
	else {
		printf("Test 'let_statements' has passed!\n");
	}
}

void 
test_token () {
	char *input_raw = 
		"let five = 5;\n"
		"let ten = 10;\n"
		"\n"	
		"let add = fn(x, y) {\n"
		"    x + y;\n"
		"};\n"
		"\n"
		"let result = add(five, ten);\n"
		"\n"
		"!-/*5;\n"
		"5 < 10 > 5;\n"
		"\n"
		"if (5 < 10) {\n"
		"    return true;\n"
		"} else {\n"
		"    return false;\n"
		"}\n"
		"\n"
		"10 == 10;\n"
		"10 != 9;\n";
	char *input = strdup(input_raw);

	bool passed = true;

	token_t tests[] = {
		{TOKEN_LET, "let", 0, 0},
		{TOKEN_IDENT, "five", 0, 4},
		{TOKEN_ASSIGN, "=", 0, 9},
		{TOKEN_INT, "5", 0, 11},
		{TOKEN_SEMICOLON, ";", 0, 12},
		{TOKEN_LET, "let", 1, 0},
		{TOKEN_IDENT, "ten", 1, 4},
		{TOKEN_ASSIGN, "=", 1, 8},
		{TOKEN_INT, "10", 1, 10},
		{TOKEN_SEMICOLON, ";", 1, 12},
		{TOKEN_LET, "let", 3, 0},
		{TOKEN_IDENT, "add", 3, 4},
		{TOKEN_ASSIGN, "=", 3, 8},
		{TOKEN_FUNCTION, "fn", 3, 10},
		{TOKEN_LPAREN, "(", 3, 12},
		{TOKEN_IDENT, "x", 3, 13},
		{TOKEN_COMMA, ",", 3, 14},
		{TOKEN_IDENT, "y", 3, 16},
		{TOKEN_RPAREN, ")", 3, 17},
		{TOKEN_LBRACE, "{", 3, 19},
		{TOKEN_IDENT, "x", 4, 4},
		{TOKEN_PLUS, "+", 4, 6},
		{TOKEN_IDENT, "y", 4, 8},
		{TOKEN_SEMICOLON, ";", 4, 9},
		{TOKEN_RBRACE, "}", 5, 0},
		{TOKEN_SEMICOLON, ";", 5, 1},
		{TOKEN_LET, "let", 7, 0},
		{TOKEN_IDENT, "result", 7, 4},
		{TOKEN_ASSIGN, "=", 7, 11},
		{TOKEN_IDENT, "add", 7, 13},
		{TOKEN_LPAREN, "(", 7, 16},
		{TOKEN_IDENT, "five", 7, 17},
		{TOKEN_COMMA, ",", 7, 21},
		{TOKEN_IDENT, "ten", 7, 23},
		{TOKEN_RPAREN, ")", 7, 26},
		{TOKEN_SEMICOLON, ";", 7,  27},
		{TOKEN_BANG, "!", 9, 0},
		{TOKEN_MINUS, "-", 9, 1},
		{TOKEN_SLASH, "/", 9, 2},
		{TOKEN_ASTERISK, "*", 9, 3},
		{TOKEN_INT, "5", 9, 4},
		{TOKEN_SEMICOLON, ";", 9, 5},
		{TOKEN_INT, "5", 10, 0},
		{TOKEN_LT, "<", 10, 2},
		{TOKEN_INT, "10", 10, 4},
		{TOKEN_GT, ">", 10, 7},
		{TOKEN_INT, "5", 10, 9},
		{TOKEN_SEMICOLON, ";", 10, 10},
		{TOKEN_IF, "if", 12, 0},
		{TOKEN_LPAREN, "(", 12, 3},
		{TOKEN_INT, "5", 12, 4},
		{TOKEN_LT, "<", 12, 6},
		{TOKEN_INT, "10", 12, 8},
		{TOKEN_RPAREN, ")", 12, 10},
		{TOKEN_LBRACE, "{", 12, 12},
		{TOKEN_RETURN, "return", 13, 4},
		{TOKEN_TRUE, "true", 13, 11},
		{TOKEN_SEMICOLON, ";", 13, 15},
		{TOKEN_RBRACE, "}", 14, 0},
		{TOKEN_ELSE, "else", 14, 2},
		{TOKEN_LBRACE, "{", 14, 7},
		{TOKEN_RETURN, "return", 15, 4},
		{TOKEN_FALSE, "false", 15, 11},
		{TOKEN_SEMICOLON, ";", 15, 16},
		{TOKEN_RBRACE, "}", 16, 0},
		{TOKEN_INT, "10", 18, 0},
		{TOKEN_EQ, "==", 18, 3},
		{TOKEN_INT, "10", 18, 6},
		{TOKEN_SEMICOLON, ";", 18, 8},
		{TOKEN_INT, "10", 19, 0},
		{TOKEN_NEQ, "!=", 19, 3},
		{TOKEN_INT, "9", 19, 6},
		{TOKEN_SEMICOLON, ";", 19, 7},
		{TOKEN_EOF, "", 0, 0},
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

		if (token.position != expected.position) {
			passed = false;
			printf(
				"tests[%d] | position wrong. expected position '%d', got '%d'\n", 
				index,
				expected.position,
				token.position
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

	lexer_destroy(lexer);
}
