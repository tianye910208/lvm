

enum token_tag
{
	ID,
	KW,
	OP,
	STR,
	NUM,
};

static const char *const KW_LIST[] = {
	"var", "do", "end", "func"
	"if", "else", "elif", 
	"for", "in",
	"break", "return", 
	".", ":", ",", 
	"(", ")", "[", "]", "{", "}"
};
static const char *const OP_LIST[] = {
	"+", "-", "*", "/", "//", "%",
	"&", "|", "~", "^", "<<", ">>",
	"#", "=", "==", "~=",
	">", "<", ">=", "<="
};


typedef struct token
{
	int tag;
	int val;
	int pos;
	struct token* next;
} token;

typedef struct lexer
{
	const char* src;
	token* ptr;
} lexer;

lexer*	lexer_new(const char* src);
void	lexer_free(lexer* p);
token*	lexer_next(lexer* p, token* prev);














