
enum {
	DB_SYM = 128, DB_STR, DB_NUM,
	KW_VAR, KW_DEF, KW_DO, KW_END, KW_BREAK, KW_RETURN,
	KW_IF, KW_THEN, KW_ELSE, KW_ELIF, KW_FOR, KW_IN, 
	KW_AND, KW_NOT, KW_OR, KW_TRUE, KW_FALSE,
	OP_DIV, OP_SHL, OP_SHR, OP_EQ, OP_NE, OP_GE, OP_LE
};


typedef struct token {
	int				id;
	int				val;
	char*			src;
	int				line;
	struct token*	next;
} token;

typedef struct lexer {
	char*		src;
	char*		sym;
	char*		str;
	token*		ptr;
} lexer;

lexer*	lexer_new(char* src);
void	lexer_free(lexer* p);
token*	lexer_next(lexer* p, token* prev);














