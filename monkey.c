
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "lexer.h"

#define LINE_SIZE 80

int main () {
	
	char line[LINE_SIZE];

	do {
		printf("\033[1;33m>>\033[0m ");
		fgets(line, LINE_SIZE, stdin); 

		lexer_t *l = lexer_create(line);

		token_t token;
		while ((token = lexer_next_token(l)).type != TOKEN_EOF) {
			printf(
				"type = %-10s literal = %-10s\n", 
				token_name(token.type), 
				token.literal
			);
		}
	} while (strncmp(line, "quit", 4) != 0);
}
