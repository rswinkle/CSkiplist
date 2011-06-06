/*
 * main.c
 *
 *  Created on: May 16, 2011
 *      Author: Robert Winkler
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Automated.h>
#include "skiplist.h"


extern void skiplist_i_test();
extern void skiplist_iv_test();


CU_TestInfo skiplist_tests[] = {
	{ "skiplist_i",			skiplist_i_test },
	{ "skiplist_iv",		skiplist_iv_test },
};





CU_SuiteInfo skiplist_suites[] = {
  { "Skiplist", NULL, NULL, skiplist_tests },
  CU_SUITE_INFO_NULL,
};



int main()
{

	if (CUE_SUCCESS != CU_initialize_registry())
	      return CU_get_error();

	CU_ErrorCode error = CU_register_suites(skiplist_suites);

	if( error != CUE_SUCCESS )
		fprintf(stderr, "wtf!");


	CU_automated_run_tests();
	fprintf(stdout, "%d\n", CU_get_error());

	CU_cleanup_registry();

	return CU_get_error();

}
