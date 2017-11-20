#include <stdlib.h>
#include <string.h>
#include "lexer.h"


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





lexer* lexer_new(const char* src) {
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

token* lexer_token(int id, int val, const char* src) {
	token* t = malloc(sizeof(token));
	t->id = id;
	t->val = val;
	t->src = src;
	return t;
}

token* lexer_next(lexer* p, token* prev) {
	if (prev && prev->next)
		return prev->next;

	const char* src = prev ? prev->src : p->src;

	while (is_space(*src)) src++;
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
			return lexer_token(DB_STR, p2-p1, src++);
		}
		case '0':
			if (*src == 'x') {
				unsigned int n = 0;
				while (*++src) {
					if (is_alhex(*src))
						n = (n << 4) + to_digit(*src);
					else
						break;
					
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



















