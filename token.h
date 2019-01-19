
#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
	TOKEN_ILLEGAL = 0, 
	TOKEN_EOF,

	TOKEN_IDENT, 
	TOKEN_INT,

	/* OPERATORS */
	TOKEN_ASSIGN, 		/*  =  */
	TOKEN_PLUS, 		/*  +  */
	TOKEN_MINUS,		/*  -  */
	TOKEN_BANG,			/*  !  */
	TOKEN_ASTERISK,		/*  *  */
	TOKEN_SLASH,		/*  /  */

	TOKEN_EQ,			/*  ==  */
	TOKEN_NEQ,			/*  !=  */

	TOKEN_LT,			/*  <  */
	TOKEN_GT,			/*  >  */

	/* DELIMITERS */
	TOKEN_COMMA, 		/*  ,  */
	TOKEN_SEMICOLON, 	/*  ;  */

	TOKEN_LPAREN,		/*  (  */
	TOKEN_RPAREN,		/*  )  */
	TOKEN_LBRACE,		/*  {  */
	TOKEN_RBRACE,		/*  }  */

	/* KEYWORDS */
	TOKEN_FUNCTION,
	TOKEN_LET,
	TOKEN_TRUE,
	TOKEN_FALSE,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_RETURN
} tokentype_t;

typedef struct {
	tokentype_t type;		
	char *literal;		

	// the position relative to the current line of the token
	unsigned int position;	
} token_t;


char 	   *token_name 			(tokentype_t type);
token_t 	token_create 		(tokentype_t type, char *ch, int pos); 
void 		token_destroy 		(token_t *token);
token_t		token_dup			(token_t token);
tokentype_t token_lookup_ident 	(char *identifier);


#endif /* TOKEN_H */

