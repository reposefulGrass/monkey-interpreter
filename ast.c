
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "statement.h"
#include "expression.h"

program_t *
ast_program_create () {
	program_t *program = (program_t *) malloc(sizeof(program_t));
    if (program == NULL) {
        fprintf(stderr, "ERROR in 'ast_program_create': Could not allocate 'program'!\n");
        exit(EXIT_FAILURE);
    }

	list stmts;
	ll_initialize(&stmts);	

	program->statements = stmts;	

	return program;
}


void
ast_program_destroy (program_t *program) {
	ll_destroy(&program->statements, statement_destroy);
	free(program);
}


char *
ast_program_string (program_t *program) {
    list stmt_strings;
    int num_chars = 0;

    ll_initialize(&stmt_strings);

    list cursor = NULL;
    while ((cursor = ll_iterator(program->statements, cursor)) != NULL) {
        statement_t *stmt = (statement_t *) cursor->data;

        char *temp = stmt->string(stmt);
        ll_append(&stmt_strings, (void *) temp);
        num_chars += strlen(temp); 
    }

    char *buffer = (char *) malloc(sizeof(char) * num_chars + 1);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR in 'ast_program_string': Failed to allocate 'buffer'!");
        exit(EXIT_FAILURE);
    }
    buffer[0] = '\0';

    cursor = NULL;
    while ((cursor = ll_iterator(stmt_strings, cursor)) != NULL) {
        char *str = (char *) cursor->data;
        strcat(buffer, str);
    }

    ll_destroy(&stmt_strings, free);
    return buffer;
}
