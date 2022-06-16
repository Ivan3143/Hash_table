#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <locale.h>

#define _CRT_SECURE_NO_WARNINGS
#define HASHSIZE 21
#define KEY(s1, s2) (strcmp(s1, s2) == 0)
#define PUBKEY(s) free(s)
#define PVAL(s) free(s)
#define SVAL(at,s) at = strdup(s)
#define SKEY(at,s) at = strdup(s)

typedef char* VAL;
typedef char* KEY;

struct node {
	struct node* next;
	KEY key;
	VAL val;
} *table[HASHSIZE];
unsigned int hash;



int HASHFUNC(KEY key) {
	unsigned int i = 0; char* keysrc = key;
	while (*key) {
		i = (i « 1) | (i » 15);
		i ^= *key++;
	}
	hash = i % HASHSIZE;
	printf("Ðåçóëüòàò îò õåø ôóíêöèè (%s) : %d\n", keysrc, hash);
	return hash;
}




struct node* receive(KEY key) {
	struct node* p;
	for (p = table[HASHFUNC(key)]; p; p = p->next)
		if (KEY(p->key, key))
			return p;
	return NULL;
}



void set(KEY key, VAL val) {
	struct node* p;
	if ((p = receive(key)) == NULL) {
		if (!(p = (struct node*)malloc(sizeof(*p)))) return;
		SKEY(p->key, key);
		p->next = table[hash];
		table[hash] = p;
	}
	else
		PVAL(p->val);
	SVAL(p->val, val);
}



struct node* receive(KEY key) {
	struct node* p;
	for (p = table[HASHFUNC(key)]; p; p = p->next)
		if (KEY(p->key, key))
			return p;
	return NULL;
}






int del(KEY key) {
	int indx = HASHFUNC(key);
	struct node* p, * prev = NULL;
	if ((p = table[indx]) == NULL) return 0;
	for (; p; prev = p, p = p->next)
		if (KEY(p->key, key)) {
			PVAL(p->val); PUBKEY(p->key);
			if (p == table[indx])
				table[indx] = p->next;
			else prev->next = p->next;
			free((void*)p); return 1;
		}
	return 0;
}





void printcell(struct node* ptr) {
	putchar('(');
	printf("%s", ptr->key); putchar(',');
	printf("%s", ptr->val); putchar(')');
}
void print() {
	register int i; struct node* p;
	printf("Hash table:\n");
	for (i = 0; i < HASHSIZE; i++)
		if ((p = table[i]) != NULL) {
			printf("%d: ", i);
			for (; p; p = p->next)
				printcell(p), putchar(' ');
			putchar('\n');
		}
}




struct celliter {
	int point; struct node* ptr;
};




struct node* nextpair(struct celliter* x) {
	struct node* result;
	while ((result = x->ptr) == NULL) {
		if (++(x->point) >= HASHSIZE)
			return NULL;
		x->ptr = table[x->point];
	}
	x->ptr = result->next; return result;
}




struct node* resetiter(struct celliter* x) {
	x->point = (-1); x->ptr = NULL;
	return nextpair(x);
}



