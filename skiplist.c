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


static node_ii nil;
static nodeptr NIL = &nil;

int randomlevel()
{
	short level = 0;
	while (rand()/(float)RAND_MAX < P && level <= MAX_LEVEL)
		level++;

	return level;
}

skiplist_i* slist_i()
{
	srand( time(NULL) );
	NIL->key = INT_MAX;

	skiplist_i *list = malloc(sizeof(skiplist_i));
	if (list == NULL) {

		//error
	}
	list->level = 0;
	list->head = newnode_ii(MAX_NUM_LEVELS);
	if (list->head == NULL) {

		//error
	}
	int i;
	for (i=0; i<=MAX_LEVEL; i++) list->head->forward[i] = NIL;

	return list;
}



int searchi(skiplist_i *list, int key, int* value)
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

		nodeptr q = newnode_ii(lvl);		//should I bother checking these allocations?
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
	int i, lvl;

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
		while (list->level > 0 && list->head->forward[list->level] == NIL)
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
	while (i != NIL) {
		j = i->forward[0];
		free(i);
		i = j;
	}

	free(list);
}
















