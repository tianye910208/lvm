#include <stdio.h>
#include "cc.h"

char* load(const char* filename, int* len) {
	FILE* fp = fopen(filename, "r");
	if (!fp)
		return -1;
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	char* buf = malloc(size + 1);
	memset(buf, 0, size + 1);
	fread(buf, 1, size, fp);

	fclose(fp);
	printf("load %s\n", filename);

	*len = (int)size;
	return buf;
}

int main(int argc, char* argv) {
	int len;
	char* buf = load("../../doc/sample.txt", &len);

	printf("len: %d\n");
	printf("buf: %s\n", buf);

	lexer* p = lexer_new(buf);

	token* t = NULL;
	while (t = lexer_next(p, t)) {
		if (t->id < 128)
			printf("%c\n", t->id);
		else if (t->id == DB_SYM || t->id == DB_STR) {
			char c = *t->src;
			*t->src = '\0';
			printf("%s\n", (char*)t->ptr);
			*t->src = c;
		}
		else if (t->id == DB_NUM) {
			printf("%f\n", t->val);
		}
		else
			printf("<%d>\n", t->id);
	}

	free(buf);

	system("pause");
	return 0;
}




