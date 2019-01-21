
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "statement.h"
#include "expression.h"

program_t *
ast_program_create () {
	program_t *program = (program_t *) malloc(sizeof(program_t));

	list new;
	ll_initialize(&new);	

	program->statements = new;	

	return program;
}

void
ast_program_destroy (program_t *program) {
	ll_destroy(&program->statements, statement_destroy);
	free(program);
}


