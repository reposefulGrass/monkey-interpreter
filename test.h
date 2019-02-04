
#ifndef TEST_H
#define TEST_H

#define CHECK_PARSER_ERRORS(parser, label) \
    if (parser_check_errors(parser)) { \
        passed = false; \
        goto label; \
    }

#define CHECK_PROGRAM_NOT_NULL(program, label) \
    if ((program) == NULL) { \
        passed = false; \
        printf("ERROR: Failed ot initialize 'program'!\n"); \
        goto label; \
    }

#define CHECK_LIST_IS_PROPER(list, len, label) \
    int length = ll_length((list)); \
    if (length != (len)) { \
        passed = false; \
        printf("ERROR: Length of list != '%d', instead it is '%d'!\n", (len), length); \
        goto label; \
    }

#define CHECK_STATEMENT_TYPE(stmt_type, expected_type, label) \
    if ((stmt_type) != expected_type) { \
        passed = false; \
        printf("ERROR: Statement type not '%d', got '%d' instead!\n", expected_type, stmt_type); \
        label; \
    }

#define CHECK_EXPRESSION_TYPE(expr_type, expected_type, label) \
    if ((expr_type) != expected_type) { \
        passed = false; \
        printf("ERROR: Expression type not '%d', got '%d' instead!\n", expected_type, expr_type); \
        label; \
    }





#endif /* TEST_H */
