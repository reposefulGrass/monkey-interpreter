
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "lexer.h"
#include "token.h"

// 'input' must be a DYNAMICALLY allocated string.
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
	l->curr_line = 0;
	l->nth_char = -1;

	lexer_read_char(l);

	return l;
}

void
lexer_destroy (lexer_t *l) {
	CHECK_LEXER_NULL(l);
	free(l->input);
	free(l);
}

void 
lexer_read_char (lexer_t *l) {
	CHECK_LEXER_NULL(l);

	if (l->read_pos >= l->length) {
		l->ch = '\0';
	}
	else {
		l->ch = l->input[l->read_pos];
	}

	l->nth_char++;
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
	CHECK_LEXER_NULL(l);

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
	CHECK_LEXER_NULL(l);

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
	CHECK_LEXER_NULL(l);

	while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
		if (l->ch == '\n') {
			l->curr_line++;
			l->nth_char = -1;
		}
		lexer_read_char(l);
	}
}

char
lexer_peek_char (lexer_t *l) {
	CHECK_LEXER_NULL(l);

	if (l->read_pos >= l->length) {
		return '\0';
	}

	return l->input[l->read_pos];
}

token_t 
lexer_next_token (lexer_t *l) {
	CHECK_LEXER_NULL(l);

	token_t tok;

	lexer_skip_whitespace(l);

	switch (l->ch) {
		case '=':
			if (lexer_peek_char(l) == '=') {
				lexer_read_char(l);
				tok = token_create(TOKEN_EQ, "==", l->nth_char - 1, l->curr_line);
			}		
			else {
				tok = token_create(TOKEN_ASSIGN, "=", l->nth_char, l->curr_line);
			}
			break;

		case '+':
			tok = token_create(TOKEN_PLUS, "+", l->nth_char, l->curr_line);
			break;

		case '-':
			tok = token_create(TOKEN_MINUS, "-", l->nth_char, l->curr_line);
			break;

		case '!':
			if (lexer_peek_char(l) == '=') {
				lexer_read_char(l);
				tok = token_create(TOKEN_NEQ, "!=", l->nth_char - 1, l->curr_line);
			}
			else {
				tok = token_create(TOKEN_BANG, "!", l->nth_char, l->curr_line);
			}
			break;

		case '/':
			tok = token_create(TOKEN_SLASH, "/", l->nth_char, l->curr_line);
			break;

		case '*':
			tok = token_create(TOKEN_ASTERISK, "*", l->nth_char, l->curr_line);
			break;

		case '<':
			tok = token_create(TOKEN_LT, "<", l->nth_char, l->curr_line);
			break;

		case '>':
			tok = token_create(TOKEN_GT, ">", l->nth_char, l->curr_line);
			break;

		case ';':
			tok = token_create(TOKEN_SEMICOLON, ";", l->nth_char, l->curr_line);
		break;

		case ',':
			tok = token_create(TOKEN_COMMA, ",", l->nth_char, l->curr_line);
		break;

		case '(':
			tok = token_create(TOKEN_LPAREN, "(", l->nth_char, l->curr_line);
		break;	  

		case ')':
			tok = token_create(TOKEN_RPAREN, ")", l->nth_char, l->curr_line);
		break;

		case '{':
			tok = token_create(TOKEN_LBRACE, "{", l->nth_char, l->curr_line);
		break;

		case '}':
			tok = token_create(TOKEN_RBRACE, "}", l->nth_char, l->curr_line);
		break;

		case '\0':
			tok.type = TOKEN_EOF;
			tok.literal = ""; /* "" (EMPTY) */
			tok.position = 0;
			tok.line = l->curr_line;
		break;

		default:
			if (is_letter(l->ch)) {
				tok.line = l->curr_line;
				tok.position = l->nth_char;
				tok.literal = lexer_read_identifier(l);				
				tok.type = token_lookup_ident(tok.literal);
				return tok;
			}
			else if (is_digit(l->ch)) {
				tok.line = l->curr_line;
				tok.position = l->nth_char;
				tok.type = TOKEN_INT;
				tok.literal = lexer_read_number(l);
				return tok;
			}
			else {
				tok = token_create(TOKEN_ILLEGAL, &l->ch, l->nth_char, l->curr_line);
			}
		break;
	}

	lexer_read_char(l);
	return tok;
}


