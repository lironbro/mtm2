/*
 * listTest.c
 *
 *  Created on: 11 באפר׳ 2016
 *      Author: Liron
 */
#include "list_mtm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define TEST_EQUALS(res_arg, a,b) 	if (res_arg && (a != b)) { \
										res_arg = false; \
									}



#define RUN_TEST(name)  printf("Running "); \
						printf(#name);		\
						printf("... ");		\
						if (!name()) { \
							printf("[FAILED]\n");		\
							return false; \
						}								\
						printf("[SUCCESS]\n");


ListElement copyInt(ListElement element){
	if(element==NULL) {
		return NULL;
	}
	int* newInt = malloc(sizeof(int));
	if(newInt==NULL) {
		return NULL;
	}
	*newInt = *((int*)element);
	return newInt;
}

void freeInt(ListElement element){
	free(element);
}

int compareInt(ListElement num1, ListElement num2){
	return ((*(int*)num2)-(*(int*)num1));
}

bool filterInt(ListElement num, ListFilterKey key){
	return num >= key;
}

bool listTest(){

	bool final=true;

	// listCreate
	// changed copyInt and freeInt to be just like in the tutorial. should work perfectly.
	List badList = listCreate(copyInt, NULL);	// needs big checking
	TEST_EQUALS(final, badList, NULL);
	badList = listCreate(NULL, freeInt);	// needs big checking
	TEST_EQUALS(final, badList, NULL);
	List list = listCreate(copyInt, freeInt);	// needs big checking
	List emptyList = listCreate(copyInt, freeInt);
	// list should be [ ]

	// listInsertFirst
	int num=1;
	TEST_EQUALS(final, listInsertFirst(badList, &num), LIST_NULL_ARGUMENT);
	// TEST_EQUALS(final, listInsertFirst(list, 9999), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, listInsertFirst(list, &num), LIST_SUCCESS);
	// list should be [1-> ]

	// listInsertLast
	TEST_EQUALS(final, listInsertLast(badList, &num), LIST_NULL_ARGUMENT);
	// TEST_EQUALS(final, listInsertLast(list, 9999), LIST_OUT_OF_MEMORY); ?
	num=3;
	TEST_EQUALS(final, listInsertLast(list, &num), LIST_SUCCESS);
	// list should be [1-> 3-> ]
	// copy should be [ ]

	// listGetSize
	TEST_EQUALS(final, listGetSize(badList), -1);
	TEST_EQUALS(final, listGetSize(NULL), -1);
	TEST_EQUALS(final, listGetSize(emptyList), 0);
	TEST_EQUALS(final, listGetSize(list), 2);

	// listCopy
	List copy = listCopy(NULL);
	TEST_EQUALS(final, copy, NULL);
	copy = listCopy(badList);
	TEST_EQUALS(final, copy, NULL);
	copy = listCopy(list);
	// list should be [1-> 3-> ]
	// copy should be [1-> 3-> ]

	// listGetFirst
	num=1;
	TEST_EQUALS(final, listGetFirst(badList), NULL);
	TEST_EQUALS(final, listGetFirst(emptyList), NULL);
	TEST_EQUALS(final, *(int*)(listGetFirst(copy)), num);
	TEST_EQUALS(final, *(int*)(listGetFirst(list)), num);

	// listGetNext
	num=3;
	TEST_EQUALS(final, listGetNext(emptyList), NULL);
	TEST_EQUALS(final, listGetNext(badList), NULL);
	TEST_EQUALS(final, *(int*)(listGetNext(copy)), num);
	TEST_EQUALS(final, *(int*)(listGetNext(list)), num);
	num=4;
	TEST_EQUALS(final, listInsertLast(list, &num), LIST_SUCCESS);
	TEST_EQUALS(final, *(int*)(listGetNext(list)), num);
	// list should be [1-> 3-> 4-> ]
	// copy should be [1-> 3-> ]


	// listGetCurrent
	num=3;
	TEST_EQUALS(final, listGetCurrent(badList), NULL);
	TEST_EQUALS(final, listGetCurrent(emptyList), NULL);
	TEST_EQUALS(final, *(int*)(listGetCurrent(copy)), num);
	num=4;
	TEST_EQUALS(final, *(int*)(listGetCurrent(list)), num);
	// list should be [1-> 3-> 4->]
	// copy should be [1-> 3-> ]

	// listInsertBeforeCurrent
	num=2;
	TEST_EQUALS(final, listInsertBeforeCurrent(badList, &num), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listInsertBeforeCurrent(emptyList, &num), LIST_INVALID_CURRENT);
	// TEST_EQUALS(final, listInsertBeforeCurrent(emptyList, 9999), LIST_OUT_OF_MEMORY); ?
	num=3;
	TEST_EQUALS(final, *(int*)listGetCurrent(copy), num)
	num=2;
	TEST_EQUALS(final, listInsertBeforeCurrent(copy, &num), LIST_SUCCESS);
	// list should be [1-> 3-> 4-> ]
	// copy should be [1-> 2-> 3-> ]

	// listInsertAfterCurrent
	num=1;
	TEST_EQUALS(final, listInsertAfterCurrent(badList, &num), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listInsertAfterCurrent(emptyList, &num), LIST_INVALID_CURRENT);
	// TEST_EQUALS(final, listInsertAfterCurrent(emptyList, 500), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, *(int*)(listGetFirst(copy)), num);
	num++;
	TEST_EQUALS(final, listInsertAfterCurrent(copy, &num), LIST_SUCCESS);
	num=5;
	TEST_EQUALS(final, listInsertAfterCurrent(list, &num), LIST_SUCCESS);
	// list should be [1-> 3-> 4-> 5-> ]
	// copy should be [1-> 2-> 3-> ]

	// listRemoveCurrent
	TEST_EQUALS(final, listRemoveCurrent(badList), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listRemoveCurrent(emptyList), LIST_INVALID_CURRENT);
	TEST_EQUALS(final, listRemoveCurrent(copy), LIST_SUCCESS);
	// list should be [1-> 2-> 3-> 4-> 5-> ]
	// copy should be [1-> 2-> ]

	// <<<<<<<<<<<<<<<<<<<<< crashes when ypou actually have to allocate some memory
	// listSort
	num=-100;
	TEST_EQUALS(final, listSort(badList, compareInt), LIST_NULL_ARGUMENT); // needs big checking
	// TEST_EQUALS(final, listSort(badList, &compareInt), LIST_OUT_OF_MEMORY); ? // needs big checking
	TEST_EQUALS(final, listSort(list, compareInt), LIST_SUCCESS);
	// list should be [1-> 2-> 3-> 4-> 5-> ]
	TEST_EQUALS(final, listInsertLast(list, &num), LIST_SUCCESS);
	num=(-1)*num;
	TEST_EQUALS(final, listInsertFirst(list, &num), LIST_SUCCESS);
	// list should be [100-> 1-> 2-> 3-> 4-> 5-> -100-> ]
	TEST_EQUALS(final, listSort(list, compareInt), LIST_SUCCESS);
	// list should be [-100-> 1-> 2-> 3-> 4-> 5-> 100-> ]	// maybe like this?


	// listFilter
	List filtered = listFilter(badList, filterInt, (ListFilterKey*)3);
	// filtered should be NULL
	filtered = listFilter(list, NULL, (ListFilterKey*)3);
	// filtered should be NULL
	filtered = listFilter(list, filterInt, NULL);
	// filtered should be NULL
	filtered = listFilter(list, filterInt, (ListFilterKey*)3);
	// filtered should be [100-> 3-> 4-> 5-> ]

	// listClear
	TEST_EQUALS(final, listClear(NULL), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listClear(copy), LIST_SUCCESS);
	// copy should be []

	// listDestroy
	listDestroy(list);
	listDestroy(copy);
	listDestroy(filtered);
	listDestroy(badList);
	listDestroy(emptyList);

	return final;

}



int main()
{
	RUN_TEST(listTest);
	return 0;
}
