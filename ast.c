
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string/dynamic_string.h"
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
	ll_destroy(&program->statements, stmt_destroy);
	free(program);
}

char *
ast_program_string (program_t *program) {
    dstr_t *program_string = ds_initialize();

    list cursor = NULL;
    while ((cursor = ll_iterator(program->statements, cursor)) != NULL) {
        stmt_t *stmt = (stmt_t *) cursor->data;

        char *str = STRING(stmt);
        ds_append(program_string, str); 
    }

    return ds_to_string(&program_string);
}

