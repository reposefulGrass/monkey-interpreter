
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "lexer.h"
#include "token.h"

lexer_t *
lexer_create (char *input) {
	lexer_t *l = (lexer_t *) malloc(sizeof(lexer_t));
	if (l == NULL) {
		printf("Failed to allocated space for lexer!\n");
		exit(EXIT_FAILURE);	
	}

	l->input = input;
	l->length = strlen(input);
	l->pos = 0;
	l->read_pos = 0;
	l->ch = '\0';

	lexer_read_char(l);

	return l;
}

void
lexer_destroy (lexer_t *l) {
	// if 'input' points to allocated memory, free it.
	free(l);
}

void 
lexer_read_char (lexer_t *l) {
	if (l->read_pos >= l->length) {
		l->ch = '\0';
	}
	else {
		l->ch = l->input[l->read_pos];
	}

	l->pos = l->read_pos;
	l->read_pos++;
}

bool
is_letter (char ch) {
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

bool
is_digit (char ch) {
	return '0' <= ch && ch <= '9';
}

char *
lexer_read_identifier (lexer_t *l) {
	int start = l->pos;

	// find the end of the identifier
	while (is_letter(l->ch)) {
		lexer_read_char(l);
	}

	int length = l->pos - start;
	char *ident = strndup(l->input + start, length);

	return ident;
}

char *
lexer_read_number (lexer_t *l) {
	int start = l->pos;

	// find the end of the identifier
	while (is_digit(l->ch)) {
		lexer_read_char(l);
	}

	int length = l->pos - start;
	
	char *number = (char *) malloc(sizeof(char) * length + 1);
	strncpy(number, l->input + start, length);	

	number[length] = '\0';

	return number;
}

void
lexer_skip_whitespace(lexer_t *l) {
	while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
		lexer_read_char(l);
	}
}

char
lexer_peek_char (lexer_t *l) {
	if (l->read_pos >= l->length) {
		return '\0';
	}

	return l->input[l->read_pos];
}

token_t 
lexer_next_token (lexer_t *l) {
	token_t tok;

	lexer_skip_whitespace(l);

	switch (l->ch) {
		case '=':
			if (lexer_peek_char(l) == '=') {
				lexer_read_char(l);
				tok = token_create(TOKEN_EQ, "==");
			}		
			else {
				tok = token_create(TOKEN_ASSIGN, "=");
			}
			break;

		case '+':
			tok = token_create(TOKEN_PLUS, "+");
			break;

		case '-':
			tok = token_create(TOKEN_MINUS, "-");
			break;

		case '!':
			if (lexer_peek_char(l) == '=') {
				lexer_read_char(l);
				tok = token_create(TOKEN_NEQ, "!=");
			}
			else {
				tok = token_create(TOKEN_BANG, "!");
			}
			break;

		case '/':
			tok = token_create(TOKEN_SLASH, "/");
			break;

		case '*':
			tok = token_create(TOKEN_ASTERISK, "*");
			break;

		case '<':
			tok = token_create(TOKEN_LT, "<");
			break;

		case '>':
			tok = token_create(TOKEN_GT, ">");
			break;

		case ';':
			tok = token_create(TOKEN_SEMICOLON, ";");
		break;

		case ',':
			tok = token_create(TOKEN_COMMA, ",");
		break;

		case '(':
			tok = token_create(TOKEN_LPAREN, "(");
		break;	  

		case ')':
			tok = token_create(TOKEN_RPAREN, ")");
		break;

		case '{':
			tok = token_create(TOKEN_LBRACE, "{");
		break;

		case '}':
			tok = token_create(TOKEN_RBRACE, "}");
		break;

		case '\0':
			tok.type = TOKEN_EOF;
			tok.literal = ""; /* "" (EMPTY) */
		break;

		default:
			if (is_letter(l->ch)) {
				tok.literal = lexer_read_identifier(l);				
				tok.type = token_lookup_ident(tok.literal);
				return tok;
			}
			else if (is_digit(l->ch)) {
				tok.type = TOKEN_INT;
				tok.literal = lexer_read_number(l);
				return tok;
			}
			else {
				tok = token_create(TOKEN_ILLEGAL, &l->ch);
			}
		break;
	}

	lexer_read_char(l);
	return tok;
}


