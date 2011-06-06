/*
 * skiplist_tests.c
 *
 *  Created on: May 16, 2011
 *      Author: robert
 */

#include "skiplist.h"
#include <CUnit/Automated.h>

#define NUM_TESTS 1000



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
	int i, j, num, tmp, value = -1;

	skiplist_i *list = slist_i();

	num = NUM_TESTS;

	for (i=0,j=0; i<num, j<4; (i+4<num)? i+=4 : (i=(i%4+1), j++)) {
		inserti(list, i, i);
	}

	for (i=0; i<num; i++) {
		tmp = searchi(list, i, &value);
		CU_ASSERT_EQUAL(tmp, 1);
		CU_ASSERT_EQUAL(value, i);
	}

	tmp = 0;
	tmp = deletei(list, --num);
	CU_ASSERT_EQUAL(tmp, 1);

	value = -1;
	tmp = searchi(list, num, &value);
	CU_ASSERT(value == -1);
	CU_ASSERT(tmp == 0);

	freei(list);
}


/** Structures used to test generic vector. */
typedef struct
{
	double d;
	int i;
	char word[30];
} t_struct;


typedef struct
{
	double d;
	int i;
	char* word;
} f_struct;



/** Useful utility function since strdup isn't standard C.*/
char* mystrdup(const char* str)
{
	char *temp = calloc(strlen(str)+1, sizeof(char));
	if( temp == NULL )
		return NULL;

	strcpy(temp, str);
	return temp;
}


t_struct * mk_t_struct(double d, int i, char* word)
{
	//could make this static since I'm just copying the values outside
	t_struct *a = malloc(sizeof(t_struct));
	a->d = d;
	a->i = i;
	strcpy(a->word, word);
	return a;
}


f_struct * mk_f_struct(double d, int i, char* word)
{
	//could make this static since I'm just copying the values outside
	f_struct *a = malloc(sizeof(f_struct));
	a->d = d;
	a->i = i;
	a->word = mystrdup(word);
	return a;
}


void free_f_struct(void *a)
{
	free(((f_struct*)a)->word);
	free(a);
}


void skiplist_iv_test()
{
	int i, j, num, tmp;
	t_struct *valuet = NULL;
	f_struct *valuef = NULL;

	skiplist_iv *list = slist_iv(NULL);
	skiplist_iv *listf = slist_iv(free_f_struct);

	num = NUM_TESTS;

	char buffer[50];
	for (i=0,j=0; i<num, j<4; (i+4<num)? i+=4 : (i=(i%4+1),j++)) {
		sprintf(buffer, "hello %d", i);

		insertiv(listf, i, mk_f_struct(i, i, buffer));
		insertiv(list, i, mk_t_struct(i, i, buffer));
	}


	for (i=0; i<num; i++) {

		sprintf(buffer, "hello %d", i);
		tmp = searchiv(list, i, (void**)&valuet);
		CU_ASSERT_EQUAL(tmp, 1);
		CU_ASSERT_EQUAL(valuet->i, i);
		CU_ASSERT_EQUAL(valuet->d, i);
		CU_ASSERT_STRING_EQUAL(valuet->word, buffer);

		tmp = 0;

		tmp = searchiv(listf, i, (void**)&valuef);
		CU_ASSERT_EQUAL(tmp, 1);
		CU_ASSERT_EQUAL(valuef->i, i);
		CU_ASSERT_EQUAL(valuef->d, i);
		CU_ASSERT_STRING_EQUAL(valuef->word, buffer);
	}

	//test delete by deleting last element of each list and searching for it
	tmp = 0;
	tmp = deleteiv(list, --num);
	CU_ASSERT_EQUAL(tmp, 1);

	valuet = NULL;
	tmp = searchiv(list, num, (void**)&valuet);
	CU_ASSERT(tmp == 0);
	CU_ASSERT(valuet == NULL);


	tmp = 0;
	tmp = deleteiv(listf, num);
	CU_ASSERT_EQUAL(tmp, 1);

	valuef = NULL;
	tmp = searchiv(listf, num, (void**)&valuef);
	CU_ASSERT(tmp == 0);
	CU_ASSERT(valuef == NULL);


	freeiv(list);
	freeiv(listf);
}

