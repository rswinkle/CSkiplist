/*
 * skiplist.h
 *
 *  Created on: May 16, 2011
 *      Author: Robert Winkler
 */

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef struct node_ii_struct* nodeptr;

typedef struct node_ii_struct
{
	int key;
	int value;
	nodeptr forward[1];
} node_ii;


typedef struct
{
	int level;
	nodeptr head;
	nodeptr NIL;
} skiplist_i;


typedef struct node_iv_struct* nodeivptr;

typedef struct node_iv_struct
{
	int key;
	void *value;
	nodeivptr forward[1];
} node_iv;


typedef struct
{
	int level;
	nodeivptr head;
	nodeivptr NIL;
	void (*free_elem)(void*);
} skiplist_iv;





int randomlevel();

//skiplist_i
skiplist_i* slist_i();
int searchi(skiplist_i *list, int key, int *value);
int inserti(skiplist_i *list, int key, int value);
int deletei(skiplist_i *list, int key);
void freei(skiplist_i *list);



//skiplist_iv
skiplist_iv* slist_iv(void (*free_elem)(void*));
int searchiv(skiplist_iv *list, int key, void **value);
int insertiv(skiplist_iv *list, int key, void *value);
int deleteiv(skiplist_iv *list, int key);
void freeiv(skiplist_iv *list);


#endif /* SKIPLIST_H_ */
