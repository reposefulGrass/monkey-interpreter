
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "linked_list/linked_list.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "statement.h"
#include "expression.h"
#include "test.h"

// helper tests
bool test_number (expr_t *expr, int expected_number);
bool test_identifier (expr_t *expr, char *expected_value);
bool test_boolean (expr_t *expr, bool value);
bool test_infix_expression (expr_t *expr, void *left, char *op, void *right, char *type);

void test_token ();
void test_let_stmts ();
void test_return_stmts ();
void test_stmt_string ();
void test_identifier_expr();
void test_number_expr();
void test_prefix_expr();
void test_infix_expr();
void test_operator_precedence();
void test_boolean_expr();

int 
main () {
    //test_token();
    //test_let_stmts();
    //test_return_stmts();
    //test_stmt_string();
    //test_identifier_expr();
    //test_number_expr();
    //test_prefix_expr();
    //test_infix_expr();
    //test_operator_precedence();
    //test_boolean_expr();
}

bool
test_boolean (expr_t *expr, bool value) {
    if (expr->type != EXPRESSION_BOOLEAN) {
        printf("ERROR: expr->type != EXPR_BOOLEAN!\n");
        return false;
    }

    char *expected = (value) ? "true" : "false";
    char *actual = TOKEN_LITERAL(expr);
    if (strcmp(actual, expected) != 0) {
        printf(
            "ERROR: TOKEN_LITERAL(expr) != '%s', instead got '%s'!\n",
            expected, actual
        );
        return false;
    }

    boolean_t boolean = EXPR_BOOLEAN(expr);
    if (boolean.value != value) {
        printf("ERROR: boolean.value != value!\n");
        return false;
    }

    return true;
}

void
test_boolean_expr () {
    bool passed = true;
    char *input = strdup("true;false;");

    lexer_t *lexer = lexer_create(input);
    parser_t *parser = parser_create(lexer);
    program_t *program = parser_parse_program(parser);

    TEST_CHECK_PARSER_ERRORS(parser, boolean_expr_end)
    TEST_CHECK_PROGRAM_NOT_NULL(program, boolean_expr_end)
    TEST_CHECK_LIST_LEN(program->statements, 2, boolean_expr_end) // 2?

    stmt_t *first_stmt = (stmt_t *) program->statements->data;
    TEST_CHECK_STATEMENT_TYPE(first_stmt->type, STATEMENT_EXPRESSION, goto boolean_expr_end)
    expr_t *first_expr = STMT_EXPR(first_stmt).expr;
    TEST_CHECK_EXPRESSION_TYPE(first_expr->type, EXPRESSION_BOOLEAN, goto boolean_expr_end)

    stmt_t *second_stmt = (stmt_t *) NEXT(program->statements)->data;
    TEST_CHECK_STATEMENT_TYPE(second_stmt->type, STATEMENT_EXPRESSION, goto boolean_expr_end)
    expr_t *second_expr = STMT_EXPR(second_stmt).expr;
    TEST_CHECK_EXPRESSION_TYPE(second_expr->type, EXPRESSION_BOOLEAN, goto boolean_expr_end)

    passed = test_boolean(first_expr, true);
    passed = test_boolean(second_expr, false);

boolean_expr_end:
    ast_program_destroy(program);
    parser_destroy(parser); 
    
    if (passed == false) {
        printf("Test 'boolean_expr' has failed!\n");
    }
    else {
        printf("Test 'boolean_expr' has passed!\n");
    }  
}

bool
test_number (expr_t *expr, int expected_number) {
    if (expr->type != EXPRESSION_NUMBER) {
        printf(
            "ERROR: expr->type != EXPRESSION_NUMBER, instead got '%d'!\n", 
            expr->type
        );
        return false;
    }

    number_t number = EXPR_NUMBER(expr);
    if (number.value != expected_number) {
        printf(
            "ERROR: number.value != '%d', instead got '%d'!\n",
            expected_number, number.value
        );
        return false;
    }

    char expected[10];
    snprintf(expected, 10, "%d", expected_number);

    char *actual = TOKEN_LITERAL(expr);
    if (strcmp(actual, expected) != 0) {
        printf(
            "ERROR: TOKEN_LITERAL(expr) != '%s', instead got '%s'!\n",
            expected, actual
        );
        return false;
    }

    return true;
}

bool
test_identifier (expr_t *expr, char *expected) {
    if (expr->type != EXPRESSION_IDENTIFIER) {
        printf(
            "ERROR: expr->type != EXPRESSION_IDENTIFIER, instead got '%d'!\n", 
            expr->type
        );
        return false;
    }

    identifier_t ident = EXPR_IDENT(expr);
    if (strcmp(ident.value, expected) != 0) {
        printf(
            "ERROR: ident.value != '%s', instead got '%s'!\n",
            expected, ident.value
        );
        return false;
    }

    char *actual = TOKEN_LITERAL(expr);
    if (strcmp(actual, expected) != 0) {
        printf(
            "ERROR: TOKEN_LITERAL(expr) != '%s', instead got '%s'!\n",
            expected, actual
        );
        return false;
    }

    return true;
}

// please forgive me
bool 
test_infix_expression (expr_t *expr, void *left, char *op, void *right, char *type) {
    if (expr->type != EXPRESSION_INFIX) {
        printf(
            "ERROR: expr->type != EXPRESSION_INFIX, instead got '%d'!\n", 
            expr->type
        );
        return false;
    } 

    infix_t infix = EXPR_INFIX(expr);
    if (strcmp(type, "identifier") == 0) {
        if (! test_identifier(infix.left_expr, (char *) left)) {
            return false;
        }
        if (! test_identifier(infix.right_expr, (char *) right)) {
            return false;
        }
    }
    else if (strcmp(type, "number") == 0) {
        if (! test_number(infix.left_expr, *((int *) left))) {
            return false;
        }
        if (! test_number(infix.right_expr, *((int *) right))) {
            return false;
        }
    }
    else {
        printf("ERROR: Unknown meta type '%s'!\n", type);
        exit(EXIT_FAILURE);
    }

    if (strcmp(infix.operator, op) != 0) {
        printf(
            "ERROR: infix.operator is not '%s', got '%s' instead!\n", 
            op, infix.operator
        );
        return false;
    }

    return true;
}

typedef struct {
    char *input;
    char *expected; // output of program_string()
} expr_test_t;

void
test_operator_precedence () {
    bool passed = true;

    expr_test_t tests[] = {
        { // test #0
            "-a * b",
            "((-a) * b)" 
        },
        { // test #1
            "!-a",
            "(!(-a))" 
        },
        { // test #2
            "a + b + c",
            "((a + b) + c)" 
        },
        { // test #3
            "a + b - c",
            "((a + b) - c)" 
        },
        { // test #4
            "a * b * c",
            "((a * b) * c)" 
        },
        { // test #5
            "a * b / c", 
            "((a * b) / c)"
        },
        { // test #6
            "a + b / c",
            "(a + (b / c))" 
        },
        { // test #7
            "a + b * c + d / e - f",
            "(((a + (b * c)) + (d / e)) - f)" 
        },
        { // test #8
            "3 + 4; -5 * 5",
            "(3 + 4)((-5) * 5)"
        },
        { // test #9
            "5 > 4 == 3 < 4",
            "((5 > 4) == (3 < 4))"
        },
        { // test #10
            "5 < 4 != 3 < 4",
            "((5 < 4) != (3 < 4))" 
        },
        { // test #11
            "3 + 4 * 5 == 3 * 1 + 4 * 5",
            "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))" 
        },
        { // test #12
            "true",
            "true",   
        },
        { // test #13
            "false",
            "false"
        }, 
        { // test #14
            "3 > 5 == false",
            "((3 > 5) == false)"
        },
        { // test #15
            "3 < 5 == true",
            "((3 < 5) == true)"
        }
    };

    printf("==== [Test] 'operator_precedence' ====\n");

    int size = sizeof(tests) / sizeof(expr_test_t);
    for (int i = 0; i < size; i++) {
        expr_test_t test = tests[i];

        char *input = strdup(test.input);

        lexer_t *lexer = lexer_create(input);
        parser_t *parser = parser_create(lexer);
        program_t *program = parser_parse_program(parser);

        TEST_CHECK_PARSER_ERRORS(parser, expr_string_end)
        TEST_CHECK_PROGRAM_NOT_NULL(program, expr_string_end)

        char *actual = ast_program_string(program);
        //printf("  %s\n", actual);
        if (strcmp(actual, test.expected) != 0) {
            printf("Error: Expected '%s', got '%s' instead.\n", test.expected, actual);
        } 
        free(actual);

expr_string_end:
        ast_program_destroy(program);
        parser_destroy(parser); 

        if (passed == false) {
            printf("  Test #%d has failed!\n", i);
        }
        else {
            printf("  Test #%d has passed!\n", i);
        }
    }
}

typedef struct {
    char *input;
    int left;
    char *operator;
    int right;
} infix_test_t;

void
test_infix_expr () {
    bool passed = true;

    infix_test_t tests[] = {
        /* Test #0 */  {"5 + 5;",  5, "+",  5},
        /* Test #1 */  {"5 - 5;",  5, "-",  5},
        /* Test #2 */  {"5 * 5;",  5, "*",  5},
        /* Test #3 */  {"5 / 5;",  5, "/",  5},
        /* Test #4 */  {"5 > 5;",  5, ">",  5},
        /* Test #5 */  {"5 < 5;",  5, "<",  5},
        /* Test #6 */  {"5 == 5;", 5, "==", 5},
        /* Test #7 */  {"5 != 5;", 5, "!=", 5},
        /* Test #8 */  {"true == true;",  true,  "==", true},
        /* Test #9 */  {"false != true;", false, "!=", true},
        /* Test #10 */ {"false == false", false, "==", false}
    };

    printf("==== [Test] 'infix_expr' ====\n");

    int size = sizeof(tests) / sizeof(infix_test_t);
    for (int index = 0; index < size; index++) {
        infix_test_t test = tests[index];

        char *input = strdup(test.input);

        lexer_t *lexer = lexer_create(input);
        parser_t *parser = parser_create(lexer);
        program_t *program = parser_parse_program(parser);

        TEST_CHECK_PARSER_ERRORS(parser, infix_expr_end)
        TEST_CHECK_PROGRAM_NOT_NULL(program, infix_expr_end)
        TEST_CHECK_LIST_LEN(program->statements, 1, infix_expr_end)

        stmt_t *stmt = (stmt_t *) program->statements->data;
        TEST_CHECK_STATEMENT_TYPE(stmt->type, STATEMENT_EXPRESSION, goto infix_expr_end)

        expr_t *expr = STMT_EXPR(stmt).expr;
        if (expr->type == EXPRESSION_NUMBER) {
            passed = test_infix_expression(expr, (void *) &test.left, test.operator, (void *) &test.right, "number");
        }
        else if (expr->type == EXPRESSION_BOOLEAN) {
            passed = test_infix_expression(expr, (void *) &test.left, test.operator, (void *) &test.right, "boolean");
        }

infix_expr_end:
        ast_program_destroy(program);
        parser_destroy(parser); 

        if (passed == false) {
            printf("  Test #%d has failed!\n", index);
        }
        else {
            printf("  Test #%d has passed!\n", index);
        }
    }
}

typedef struct {
    char *input;
    char *operator;
    int number;
} prefix_test_t;

void
test_prefix_expr () {
    bool passed = true;

    prefix_test_t tests[] = {
        /* Test #0 */ {"!5;",    "!",  5},
        /* Test #1 */ {"-15;",   "-",  15},
        /* Test #2 */ {"!true",  "!",  true},
        /* Test #3 */ {"!false", "!",  false}
    };

    printf("==== [Test] 'prefix_expr' ====\n");

    int size = sizeof(tests) / sizeof(prefix_test_t);
    for (int index = 0; index < size; index++) {
        prefix_test_t test = tests[index];

        char *input = strdup(test.input);

        lexer_t *lexer = lexer_create(input);
        parser_t *parser = parser_create(lexer);
        program_t *program = parser_parse_program(parser);

        TEST_CHECK_PARSER_ERRORS(parser, pe_free_resources)
        TEST_CHECK_PROGRAM_NOT_NULL(program, pe_free_resources)
        TEST_CHECK_LIST_LEN(program->statements, 1, pe_free_resources)

        stmt_t *stmt = (stmt_t *) program->statements->data;
        TEST_CHECK_STATEMENT_TYPE(stmt->type, STATEMENT_EXPRESSION, goto pe_free_resources)

        stmt_expr_t expr_stmt = STMT_EXPR(stmt);
        expr_t *expr = expr_stmt.expr;
        TEST_CHECK_EXPRESSION_TYPE(expr->type, EXPRESSION_PREFIX, goto pe_free_resources)

        prefix_t prefix = EXPR_PREFIX(expr);
        if (strcmp(prefix.operator, test.operator) != 0) {
            passed = false;
            printf(
                "ERROR: prefix.operator != '%s', instead got '%s'!\n", 
                test.operator, prefix.operator
            );
            goto pe_free_resources;
        }

        if (prefix.expr->type == EXPRESSION_NUMBER) {
            passed = test_number(prefix.expr, test.number);
        }
        else if (prefix.expr->type == EXPRESSION_BOOLEAN) {
            passed = test_boolean(prefix.expr, test.number);
        }

pe_free_resources:
        ast_program_destroy(program);
        parser_destroy(parser); 

        if (passed == false) {
            printf("  Test #%d has failed!\n", index);
        }
        else {
            printf("  Test #%d has passed!\n", index);
        }
    }
}

void 
test_number_expr () {
    bool passed = true;
    char *input_raw = "5;";
    char *input = strdup(input_raw);

    lexer_t *lexer = lexer_create(input);
    parser_t *parser = parser_create(lexer);
    program_t *program = parser_parse_program(parser);

    TEST_CHECK_PARSER_ERRORS(parser, ne_free_resources)
    TEST_CHECK_PROGRAM_NOT_NULL(program, ne_free_resources)
    TEST_CHECK_LIST_LEN(program->statements, 1, ne_free_resources)

    stmt_t *stmt = (stmt_t *) program->statements->data;
    TEST_CHECK_STATEMENT_TYPE(stmt->type, STATEMENT_EXPRESSION, goto ne_free_resources)

    stmt_expr_t expr_stmt = STMT_EXPR(stmt);
    expr_t *expr = expr_stmt.expr;
    TEST_CHECK_EXPRESSION_TYPE(expr->type, EXPRESSION_NUMBER, goto ne_free_resources);

    number_t ident = EXPR_NUMBER(expr);
    int expected_value = 5;
    if (ident.value != expected_value) {
        printf(
            "ERROR: Number value not '%d', got '%d' instead.\n",
            expected_value, ident.value
        );
        goto ne_free_resources;
    }

ne_free_resources:
    ast_program_destroy(program);
    parser_destroy(parser); 
    
    if (passed == false) {
        printf("Test 'number_expr' has failed!\n");
    }
    else {
        printf("Test 'number_expr' has passed!\n");
    }    
}

void 
test_identifier_expr () {
    bool passed = true;
    char *input_raw = "foobar;";
    char *input = strdup(input_raw);

    lexer_t *lexer = lexer_create(input);
    parser_t *parser = parser_create(lexer);
    program_t *program = parser_parse_program(parser);

    TEST_CHECK_PARSER_ERRORS(parser, identifier_end)
    TEST_CHECK_PROGRAM_NOT_NULL(program, identifier_end)
    TEST_CHECK_LIST_LEN(program->statements, 1, identifier_end)

    stmt_t *stmt = (stmt_t *) program->statements->data;
    TEST_CHECK_STATEMENT_TYPE(stmt->type, STATEMENT_EXPRESSION, goto identifier_end)

    expr_t *expr = STMT_EXPR(stmt).expr;
    TEST_CHECK_EXPRESSION_TYPE(expr->type, EXPRESSION_IDENTIFIER, goto identifier_end)

    if (test_identifier(expr, "foobar") == false) {
        passed = false;
        goto identifier_end;
    }

identifier_end:
    ast_program_destroy(program);
    parser_destroy(parser); 
    
    if (passed == false) {
        printf("Test 'identifier_expr' has failed!\n");
    }
    else {
        printf("Test 'identifier_expr' has passed!\n");
    }    
}

// whenever I can parse expr change this hardcode into input!
void
test_stmt_string () {
    bool passed = true;

    token_t token = token_create(TOKEN_LET, "let", 0, 0);

    token_t identifier_token = token_create(TOKEN_IDENT, "myVar", 0, 0);
    expr_t *identifier = expr_identifier_create(identifier_token);
    token_destroy(&identifier_token);

    token_t value_token = token_create(TOKEN_IDENT, "anotherVar", 0, 0);
    expr_t *value = expr_identifier_create(value_token);
    token_destroy(&value_token);

    stmt_t *stmt = stmt_let_create(token, identifier, value);
    token_destroy(&token);

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
        printf("Test 'stmt_string' has passed!\n");
    }
    else {
        printf("Test 'stmt_string' has failed!\n");
    }
}

void
test_return_stmts() {
    char *input_raw = 
        "return 5;\n"
        "return 10;\n"
        "return 993322;\n";
    char *input = strdup(input_raw);

    bool passed = true;

    lexer_t *lexer = lexer_create(input);
    parser_t *parser = parser_create(lexer);
    program_t *program = parser_parse_program(parser);

    TEST_CHECK_PARSER_ERRORS(parser, rs_free_resources)
    TEST_CHECK_PROGRAM_NOT_NULL(program, rs_free_resources)
    TEST_CHECK_LIST_LEN(program->statements, 3, rs_free_resources)

    list cursor = NULL;
    while ((cursor = ll_iterator(program->statements, cursor)) != NULL) {
        stmt_t *stmt = (stmt_t *) cursor->data;

        TEST_CHECK_STATEMENT_TYPE(stmt->type, STATEMENT_RETURN, continue)

        if (strcmp(stmt->token_literal(stmt), "return") != 0) {
            passed = false;
            printf(
                "stmt->token_literal(stmt) NOT 'return'; got '%s' instead.\n",
                TOKEN_LITERAL(stmt)
            );
        }
    }

rs_free_resources:
    ast_program_destroy(program);
    parser_destroy(parser);

    if (passed == false) {
        printf("Test 'return_stmts' has failed!\n");
    }
    else {
        printf("Test 'return_stmts' has passed!\n");
    }
}

bool
test_let_stmt (stmt_t *stmt, char *expected_identifier) {
    if (stmt->type != STATEMENT_LET) {
        printf(
            "ERROR: Statement type != '%d', got '%d' instead!\n", 
            STATEMENT_LET, stmt->type
        );
        return false;
    }

    if (strcmp(TOKEN_LITERAL(stmt), "let") != 0) {
        printf(
            "ERROR: stmt->token_literal() not 'let'. got '%s'\n", 
            stmt->token_literal(stmt)
        );
        return false;
    }

    stmt_let_t let_stmt = STMT_LET(stmt);

    return test_identifier(let_stmt.name, expected_identifier);
}

void
test_let_stmts() {
    char *input_raw = 
        "let x = 5;\n"
        "let y = 10;\n"
        "let foobar = 838383;\n";
    char *input = strdup(input_raw);

    bool passed = true;

    lexer_t *lexer = lexer_create(input);
    parser_t *parser = parser_create(lexer);
    program_t *program = parser_parse_program(parser);

    TEST_CHECK_PARSER_ERRORS(parser, ls_free_resources)
    TEST_CHECK_PROGRAM_NOT_NULL(program, ls_free_resources)
    TEST_CHECK_LIST_LEN(program->statements, 3, ls_free_resources)

    char *tests[] = {
        "x",
        "y",
        "foobar"
    };

    int index = 0;
    list cursor = NULL;
    while ((cursor = ll_iterator(program->statements, cursor)) != NULL) {
        stmt_t *stmt = (stmt_t *) cursor->data;
        if (!test_let_stmt(stmt, tests[index])) {
            passed = false;
            goto ls_free_resources;
        }
        
        index++;    
    }

ls_free_resources:
    ast_program_destroy(program);
    parser_destroy(parser);

    if (passed == false) {
        printf("Test 'let_stmts' has failed!\n");
    }
    else {
        printf("Test 'let_stmts' has passed!\n");
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
        {TOKEN_NUMBER, "5", 0, 11},
        {TOKEN_SEMICOLON, ";", 0, 12},
        {TOKEN_LET, "let", 1, 0},
        {TOKEN_IDENT, "ten", 1, 4},
        {TOKEN_ASSIGN, "=", 1, 8},
        {TOKEN_NUMBER, "10", 1, 10},
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
        {TOKEN_NUMBER, "5", 9, 4},
        {TOKEN_SEMICOLON, ";", 9, 5},
        {TOKEN_NUMBER, "5", 10, 0},
        {TOKEN_LT, "<", 10, 2},
        {TOKEN_NUMBER, "10", 10, 4},
        {TOKEN_GT, ">", 10, 7},
        {TOKEN_NUMBER, "5", 10, 9},
        {TOKEN_SEMICOLON, ";", 10, 10},
        {TOKEN_IF, "if", 12, 0},
        {TOKEN_LPAREN, "(", 12, 3},
        {TOKEN_NUMBER, "5", 12, 4},
        {TOKEN_LT, "<", 12, 6},
        {TOKEN_NUMBER, "10", 12, 8},
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
        {TOKEN_NUMBER, "10", 18, 0},
        {TOKEN_EQ, "==", 18, 3},
        {TOKEN_NUMBER, "10", 18, 6},
        {TOKEN_SEMICOLON, ";", 18, 8},
        {TOKEN_NUMBER, "10", 19, 0},
        {TOKEN_NEQ, "!=", 19, 3},
        {TOKEN_NUMBER, "9", 19, 6},
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
