/*
 * skiplist.c
 *
 *  Created on: May 16, 2011
 *      Author: Robert Winkler
 */
#include "skiplist.h"

#define P 0.25
#define MAX_NUM_LEVELS 16
#define MAX_LEVEL 15

#define newnode_ii(l) malloc(sizeof(node_ii)+(l)*sizeof(nodeptr))
#define newnode_iv(l) malloc(sizeof(node_iv)+(l)*sizeof(nodeivptr))



int randomlevel()
{
	short level = 0;
	while (rand()/(float)RAND_MAX < P && level <= MAX_LEVEL)
		level++;

	return level;
}

skiplist_i* slist_i()
{
	static node_ii nil;
	nil.key = INT_MAX;

	srand( time(NULL) );

	skiplist_i *list = malloc(sizeof(skiplist_i));
	list->NIL = &nil;
	list->level = 0;
	list->head = newnode_ii(MAX_NUM_LEVELS);

	int i;
	for (i=0; i<=MAX_LEVEL; i++) list->head->forward[i] = list->NIL;

	return list;
}



int searchi(skiplist_i *list, int key, int *value)
{
	nodeptr update[MAX_NUM_LEVELS];

	nodeptr x = list->head;
	int i;

	for (i=list->level; i>=0; i--) {
		while (x->forward[i]->key < key)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];

	if (x->key == key) {
		*value = x->value;
		return 1;
	} else {
		return 0;
	}
}



int inserti(skiplist_i *list, int key, int value)
{
	nodeptr update[MAX_NUM_LEVELS];

	nodeptr x = list->head;
	int i, lvl;

	for (i=list->level; i>=0; i--) {
		while (x->forward[i]->key < key)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];

	if (x->key == key)
		x->value = value;
	else {
		lvl = randomlevel();
		if (lvl > list->level) {
			lvl = ++list->level;
			update[lvl] = list->head;
		}

		nodeptr q = newnode_ii(lvl);		//I'm not going to check allocations in this project
		q->key = key;
		q->value = value;

		for (i=0; i<=lvl; i++) {
			q->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = q;
		}
	}

	return 1;
}


int deletei(skiplist_i *list, int key)
{
	nodeptr update[MAX_LEVEL];

	nodeptr x = list->head;
	int i;

	for (i=list->level; i>=0; i--) {
		while (x->forward[i]->key < key)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];

	if (x->key==key) {
		for (i=0; i<list->level; i++) {
			if (update[i]->forward[i] != x) break;
			update[i]->forward[i] = x->forward[i];
		}
		free(x);
		while (list->level > 0 && list->head->forward[list->level] == list->NIL)
			--list->level;

	} else {
		//node not found
		return 0;
	}

	return 1;
}


void freei(skiplist_i *list)
{
	nodeptr i, j;

	i = list->head;
	while (i != list->NIL) {
		j = i->forward[0];
		free(i);
		i = j;
	}

	free(list);
}


// skiplist_iv

skiplist_iv* slist_iv(void (*free_elem)(void*))
{
	static node_iv nil;
	nil.key = INT_MAX;

	srand( time(NULL) );

	skiplist_iv *list = malloc(sizeof(skiplist_iv));

	list->NIL = &nil;
	list->level = 0;
	list->head = newnode_iv(MAX_NUM_LEVELS);
	list->head->value = NULL;
	if (free_elem)
		list->free_elem = free_elem;
	else
		list->free_elem = NULL;

	int i;
	for (i=0; i<=MAX_LEVEL; i++) list->head->forward[i] = list->NIL;

	return list;
}



/** Return the pointer to value in parameter value */
int searchiv(skiplist_iv *list, int key, void **value)
{
	nodeivptr update[MAX_NUM_LEVELS];

	nodeivptr x = list->head;
	int i;

	for (i=list->level; i>=0; i--) {
		while (x->forward[i]->key < key)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];

	if (x->key == key) {
		*value = x->value;
		return 1;
	} else {
		return 0;
	}
}



int insertiv(skiplist_iv *list, int key, void *value)
{
	nodeivptr update[MAX_NUM_LEVELS];

	nodeivptr x = list->head;
	int i, lvl;

	for (i=list->level; i>=0; i--) {
		while (x->forward[i]->key < key)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];

	if (x->key == key) {
		x->value = value;
		return 0;
	} else {
		lvl = randomlevel();
		if (lvl > list->level) {
			lvl = ++list->level;
			update[lvl] = list->head;
		}

		nodeivptr q = newnode_iv(lvl);		//should I bother checking these allocations?
		q->key = key;
		q->value = value;

		for (i=0; i<=lvl; i++) {
			q->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = q;
		}
	}

	return 1;
}


int deleteiv(skiplist_iv *list, int key)
{
	nodeivptr update[MAX_LEVEL];

	nodeivptr x = list->head;
	int i;

	for (i=list->level; i>=0; i--) {
		while (x->forward[i]->key < key)
			x = x->forward[i];
		update[i] = x;
	}
	x = x->forward[0];

	if (x->key == key) {
		for (i=0; i<list->level; i++) {
			if (update[i]->forward[i] != x) break;
			update[i]->forward[i] = x->forward[i];
		}

		if (list->free_elem)
			list->free_elem(x->value);
		else
			free(x->value);

		free(x);

		while (list->level > 0 && list->head->forward[list->level] == list->NIL)
			--list->level;

	} else {
		//node not found
		return 0;
	}

	return 1;
}


void freeiv(skiplist_iv *list)
{
	nodeivptr i, j;

	i = list->head;

	if (list->free_elem) {
		//this is to handle NULL value case of head pointer
		j = i->forward[0];
		free(i);
		i = j;
		while (i != list->NIL) {
			j = i->forward[0];
			list->free_elem(i->value);
			free(i);
			i = j;
		}
	} else {
		while (i != list->NIL) {
			j = i->forward[0];
//			fprintf(stderr, "%X\n", i->value);
			free(i->value);
			free(i);
			i = j;
		}
	}
	free(list);
}











