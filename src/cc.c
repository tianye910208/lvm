#include <stdlib.h>
#include <string.h>
#include "cc.h"


const unsigned char _ASCII_BIT_MAP[128] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0.
	0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00,//0.
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//1.
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//1.
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//2.
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//2.
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,//3.
	0x68, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//3.
	0x00, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x10,//4.
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,//4.
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,//5.
	0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10,//5.
	0x00, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x10,//6.
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,//6.
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,//7.
	0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,//7.
};

#define _ASCII_BIT_ALPHA 0x10
#define _ASCII_BIT_DIGIT 0x20
#define _ASCII_BIT_ALNUM 0x30
#define _ASCII_BIT_ALHEX 0x40
#define _ASCII_BIT_SPACE 0x80
#define _ASCII_HEX_VALUE 0x0f


#define is_space(c) (_ASCII_BIT_MAP[c] & _ASCII_BIT_SPACE)//' ','\t','\v','\f'
#define is_alpha(c)	(_ASCII_BIT_MAP[c] & _ASCII_BIT_ALPHA)//a-z,A-Z,'_'
#define is_digit(c)	(_ASCII_BIT_MAP[c] & _ASCII_BIT_DIGIT)//0-9
#define is_alnum(c)	(_ASCII_BIT_MAP[c] & _ASCII_BIT_ALNUM)
#define is_alhex(c)	(_ASCII_BIT_MAP[c] & _ASCII_BIT_ALHEX)//a-f,A-F,0-9
#define to_digit(c) (_ASCII_BIT_MAP[c] & _ASCII_HEX_VALUE)





lexer* lexer_new(char* src) {
	lexer* p = malloc(sizeof(lexer));
	p->src = src;
	p->ptr = NULL;
	p->str = NULL;
	p->sym = NULL;
	return p;
}

void lexer_free(lexer* p) {
	free(p);
}

token* lexer_token(int id, float val, char* ptr, char* src, int line, token* prev) {
	token* t = malloc(sizeof(token));
	t->id = id;
	t->val = val;
	t->ptr = ptr;
	t->src = src;
	t->line = line;
	t->next = NULL;
	if (prev)
		prev->next = t;
	return t;
}

token* lexer_next(lexer* p, token* prev) {
	if (prev && prev->next)
		return prev->next;

	char* src = prev ? prev->src : p->src;
	int line = prev ? prev->line : 1;

	
	char c;
	for (;;) {
		c = *src++;
		switch (c) {
		case '\0':
			return NULL;
		case '\n': case '\r': {
			if ((*src == '\n' || *src == '\r') && *src != c) src++;
			line++;
			break;
		}
		case ' ': case '\f': case '\t': case '\v':
			break;
		case '-':
			if (*src == '-') {
				while (*src != '\n' && *src != '\r') src++;
				break;
			}
			return lexer_token(c, 0, 0, src, line, prev);
		case '/':
			if (*src == '/')
				return lexer_token(OP_DIV, 0, 0, ++src, line, prev);
			return lexer_token(c, 0, 0, src, line, prev);
		case '<':
			if (*src == '=')
				return lexer_token(OP_LE, 0, 0, ++src, line, prev);
			else if (*src == '<')
				return lexer_token(OP_SHL, 0, 0, ++src, line, prev);
			return lexer_token(c, 0, 0, src, line, prev);
		case '>':
			if (*src == '=')
				return lexer_token(OP_GE, 0, 0, ++src, line, prev);
			else if (*src == '>')
				return lexer_token(OP_SHR, 0, 0, ++src, line, prev);
			return lexer_token(c, 0, 0, src, line, prev);
		case '=':
			if (*src == '=')
				return lexer_token(OP_EQ, 0, 0, ++src, line, prev);
			return lexer_token(c, 0, 0, src, line, prev);
		case '~':
			if (*src == '=')
				return lexer_token(OP_NE, 0, 0, ++src, line, prev);
			return lexer_token(c, 0, 0, src, line, prev);
		case '"': case '\'': {
			const char* pos = src - 1;
			char s = *src;
			while (s != c) {
				if (s == '\\')
					src += 2;
				s = *src++;
			}
			return lexer_token(DB_STR, 0, pos, src, line, prev);
		}
		case '0':
			if (*src == 'x') {
				unsigned int n = 0;
				while (is_alhex(*++src))
					n = (n << 4) + to_digit(*src);

				return lexer_token(DB_NUM, n, 0, src, line, prev);
			}
		case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
			unsigned int n = to_digit(c);
			while (is_digit(*src))
				n = (n * 10) + to_digit(*src++);

			if (*src != '.')
				return lexer_token(DB_NUM, n, 0, src, line, prev);
			
			src++;
			float f = 0;
			float k = 1;
			while (is_digit(*src)) {
				k = k * 0.1f;
				f = f + k * to_digit(*src++);
			}
			return lexer_token(DB_NUM, n + f, 0, src, line, prev);
		}
		default:
			if (is_alpha(c)){
				const char* pos = src - 1;
				while (is_alnum(*src))
					src++;
				return lexer_token(DB_SYM, 0, pos, src, line, prev);
			}
			return lexer_token(c, 0, 0, src, line, prev);
		}
	}
}



















