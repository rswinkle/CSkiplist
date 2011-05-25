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


typedef struct sl_i
{
	int level;
	nodeptr head;
} skiplist_i;





int randomlevel();

//skiplist_i
skiplist_i* slist_i();
int searchi(skiplist_i *list, int key, int* value);
int inserti(skiplist_i *list, int key, int value);
int deletei(skiplist_i *list, int key);
void freei(skiplist_i *list);




#endif /* SKIPLIST_H_ */
