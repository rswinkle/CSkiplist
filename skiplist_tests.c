/*
 * skiplist_tests.c
 *
 *  Created on: May 16, 2011
 *      Author: robert
 */

#include "skiplist.h"
#include <CUnit/Automated.h>


// note the float literals are important, otherwise the integers could
// overflow when 1 is added.
int random_number(int low, int high)  //the correct random number generator for [0,hi]
{
	int d = high - low;
	// scale in range [0,1)
	float scale = rand()/(float)RAND_MAX;

	// return range [0,hi]
	scale = scale*d + 0.5 + low;
	return (int)scale; // implicit cast and truncation in return
}



void skiplist_i_test()
{
	int i, num, tmp, value = -1;

	skiplist_i *list = slist_i();
	srand(time(NULL));

	num = 50000;

	fprintf(stderr, "\n%d\t%d\n\n", sizeof(node_ii), sizeof(node_ii)+sizeof(nodeptr));

	fprintf(stderr, "\n%d\n\n", RAND_MAX);
	int *keys = malloc(sizeof(int)*num);

	for(i=0; i<num; i++)
		keys[i] = i;

	for (i=0; i<num; i++) {
		inserti(list, keys[i], keys[i]);
	}


	for (i=0; i<num; i++) {
		tmp = searchi(list, keys[i], &value);
		CU_ASSERT_EQUAL(tmp, 1);
		CU_ASSERT_EQUAL(value, keys[i]);
	}

	tmp = 0;
	tmp = deletei(list, keys[--num]);
	CU_ASSERT_EQUAL(tmp, 1);

	value = -1;
	tmp = searchi(list, keys[num], &value);
	CU_ASSERT(value == -1);
	CU_ASSERT(tmp == 0);


	free(keys);
	freei(list);
}
