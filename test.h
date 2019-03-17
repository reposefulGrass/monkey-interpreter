
#ifndef TEST_H
#define TEST_H

int __test_num = 0;
int __failed_tests = 0;

// thanks https://github.com/antirez
#define test_fail(desc, cond) do {              \
    __test_num++;                               \
    if (!(cond)) {                              \
        printf("  FAILED @%d #%d | %s\n", __LINE__, __test_num, desc); \
        __failed_tests++;                       \
    }                                           \
    } while (0); 

#define test_report() do {                      \
    printf(                                     \
        "%d tests, %d passed, %d failed\n\n",   \
        __test_num,                             \
        __test_num - __failed_tests,            \
        __failed_tests                          \
    );                                          \
    __test_num = 0;                             \
    __failed_tests = 0;                         \
    } while (0); 


// DEPRECATED (DO NOT USE | IN PROCESS OF REMOVAL)
#define TEST_CHECK_PARSER_ERRORS(parser, label) \
    if (parser_check_errors(parser)) { \
        passed = false; \
        goto label; \
    }

#define TEST_CHECK_PROGRAM_NOT_NULL(program, label) \
    if ((program) == NULL) { \
        passed = false; \
        printf("ERROR: Failed to initialize 'program'!\n"); \
        goto label; \
    }

#define TEST_CHECK_LIST_LEN(list, len, label) \
    int length = ll_length((list)); \
    if (length != (len)) { \
        passed = false; \
        printf("ERROR: Length of list != '%d', instead it is '%d'!\n", (len), length); \
        goto label; \
    }

#define TEST_CHECK_STATEMENT_TYPE(stmt_type, expected_type, label) \
    if ((stmt_type) != expected_type) { \
        passed = false; \
        printf("ERROR: Statement type not '%d', got '%d' instead!\n", expected_type, stmt_type); \
        label; \
    }

#define TEST_CHECK_EXPRESSION_TYPE(expr_type, expected_type, label) \
    if ((expr_type) != expected_type) { \
        passed = false; \
        printf("ERROR: Expression type not '%d', got '%d' instead!\n", expected_type, expr_type); \
        label; \
    }

#endif /* TEST_H */

