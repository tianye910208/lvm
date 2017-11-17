#include <stdio.h>
#include <string.h>
#include "lexer.h"


enum { 
	SPACE		= 0x01,  
    DIGIT		= 0x02, 
	CHAR		= 0x04,
	SIGN		= 0x08, 
	HEX			= 0x10
};

lexer* lexer_new(const char* src) {
	lexer* p = malloc(sizeof(lexer));
	p->src = src;
	p->ptr = NULL;
	p->str = NULL;
	p->sym = NULL;

	char* map = p->map;
	memset(map, 0, sizeof(p->map));
	
	for (char c = '0'; c <= '9'; c++)
		map[c] = DIGIT;
	for (char c = 'a'; c <= 'f'; c++)
		map[c] = CHAR|HEX;
	for (char c = 'A'; c <= 'F'; c++)
		map[c] = CHAR|HEX;
	for (char c = 'g'; c <= 'z'; c++)
		map[c] = CHAR;
	for (char c = 'G'; c <= 'Z'; c++)
		map[c] = CHAR;
	map['_'] = CHAR;
	map['\t'] = map['\v'] = map['\f'] = map['\r'] = map[' '] = SPACE;
	map['{'] = map['}'] = map['['] = map[']'] = map['('] = map[')'] = SIGN;
	map['+'] = map['-'] = map['*'] = map['/'] = map['%'] = map['^'] = SIGN;
	map['&'] = map['|'] = map['~'] = map['>'] = map['<'] = map['='] = SIGN;
	map[':'] = map['.'] = map[','] = map['#'] = SIGN;
}

void lexer_free(lexer* p) {
	free(p);
}

token* lexer_token(int id, int val, const char* src) {
	token* t = malloc(sizeof(token));
	t->id = id;
	t->val = val;
	t->src = src;
	return t;
}

token* lexer_next(lexer* p, token* prev) {
	const char* map = p->map;
	const char* src = prev ? prev->src : p->src;

	while (map[*src] & SPACE) src++;
	char c = *src++;
	for (;;) {
		switch (c){
		case '\n':
			//line++;
			break;
		case '-':
			if (*src == '-')
			{
				while (*src != '\n') src++;
				break;
			}
			return lexer_token(c, 0, src);
		case '\'':
		case '"': {
			const char* p1 = src;
			char s = *src;
			while (s != c) {
				if (s == '\\')
					src++;
				s = *src++;
			}
			const char* p2 = src;
			return lexer_token(DB_STR, p2, src++);
		}
		case '0':
			if (*src == 'x') {
				unsigned int n = 0;
				int d = 0;
				while (*++src) {
					if (map[*src] & DIGIT)
						d = *src - '0';
					else if (*src >= 'a' && *src <= 'f')
						d = *src - 'a' + 10;
					else if (*src >= 'A' && *src <= 'F')
						d = *src - 'A' + 10;
					else
						break;
					n = (n << 4) + d;
				}
				return lexer_token(DB_NUM, n, src++);
			}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			//unsigned int n = 0;
			//while (map[c] & DIGIT)
		default:
			break;
		}
	}
}



















