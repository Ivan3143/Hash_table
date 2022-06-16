#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <locale.h>
#include "hash_table.h"

#define _CRT_SECURE_NO_WARNINGS
#define HASHSIZE 21
#define KEY(s1, s2) (strcmp(s1, s2) == 0)
#define PUBKEY(s) free(s)
#define PVAL(s) free(s)
#define SVAL(at,s) at = strdup(s)
#define SKEY(at,s) at = strdup(s)

int main(void) {
	setlocale(LC_ALL, "Rus");
	struct celliter x; struct node* cl;
	char key[40], value[40]; struct node* val;
	extern FILE* popen(); FILE* file; char* s;
	file = popen("ls -s", "r");
	while (fscanf(file, "%s%s", value, key) == 2)
		set(key, value);
	pclose(file);
	for (;;) {
		printf("-> ");
		if (!fgets(key, 126, file)) break;
		if (*key == '-') {
			printf(del(key + 1) ? "ОК\n" : "ключ не найден\n");
			continue;
		}
		if (!*key || !strcmp(key, "=")) {
			print(); continue;
		}
		if ((s = strchr(key, '='))) {
			*s++ = '\0';
			set(key, s); continue;
		}
		if ((val = receive(key)) == NULL)
			printf("Такого ключа нет\n");
		else {
			printf("Значение "); printf("%s", val->val);
			putchar('\n');
		}
	}

	for (cl = resetiter(&x); cl; cl = nextpair(&x))
		printcell(cl), putchar('\n');
	print();
	return 0;
}

