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
	TEST_EQUALS(final, listInsertFirst(badList, (ListElement*)9999), LIST_NULL_ARGUMENT);
	// TEST_EQUALS(final, listInsertFirst(list, 9999), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, listInsertFirst(list, (ListElement*)1), LIST_SUCCESS);
	// list should be [1-> ]

	// listInsertLast
	TEST_EQUALS(final, listInsertLast(badList, (ListElement*)9999), LIST_NULL_ARGUMENT);
	// TEST_EQUALS(final, listInsertLast(list, 9999), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, listInsertLast(list, (ListElement*)3), LIST_SUCCESS);
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
	TEST_EQUALS(final, listGetFirst(badList), NULL);
	TEST_EQUALS(final, listGetFirst(emptyList), NULL);
	TEST_EQUALS(final, listGetFirst(copy), (ListElement*)1);
	TEST_EQUALS(final, listGetFirst(list), (ListElement*)1);

	// listGetNext
	TEST_EQUALS(final, listGetNext(emptyList), NULL);
	TEST_EQUALS(final, listGetNext(badList), NULL);
	TEST_EQUALS(final, listGetNext(copy), (ListElement*)3);
	TEST_EQUALS(final, listGetNext(list), (ListElement*)3);
	TEST_EQUALS(final, listInsertLast(list, (ListElement*)4), LIST_SUCCESS);
	TEST_EQUALS(final, listGetNext(list), (ListElement*)4);
	// list should be [1-> 3-> 4-> ]
	// copy should be [1-> 3-> ]


	// listGetCurrent
	TEST_EQUALS(final, listGetCurrent(badList), NULL);
	TEST_EQUALS(final, listGetCurrent(emptyList), NULL);
	TEST_EQUALS(final, listGetCurrent(copy), (ListElement*)3);
	TEST_EQUALS(final, listGetCurrent(list), (ListElement*)4);
	// list should be [1-> 3-> 4->]
	// copy should be [1-> 3-> ]

	// listInsertBeforeCurrent
	TEST_EQUALS(final, listInsertBeforeCurrent(badList, (ListElement*)9999), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listInsertBeforeCurrent(emptyList, (ListElement*)9999), LIST_INVALID_CURRENT);
	// TEST_EQUALS(final, listInsertBeforeCurrent(emptyList, 9999), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, listInsertBeforeCurrent(copy, (ListElement*)2), LIST_SUCCESS);
	// list should be [1-> 3-> 4-> ]
	// copy should be [1-> 2-> 3-> ]

	// listInsertAfterCurrent
	TEST_EQUALS(final, listInsertAfterCurrent(badList, (ListElement*)500), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listInsertAfterCurrent(emptyList, (ListElement*)500), LIST_INVALID_CURRENT);
	// TEST_EQUALS(final, listInsertAfterCurrent(emptyList, 500), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, listGetFirst(copy), (ListElement*)1);
	TEST_EQUALS(final, listInsertAfterCurrent(copy, (ListElement*)2), LIST_SUCCESS);
	TEST_EQUALS(final, listInsertAfterCurrent(list, (ListElement*)5), LIST_SUCCESS);
	// list should be [1-> 2-> 3-> 4-> 5-> ]
	// copy should be [1-> 2-> 3-> ]

	// listRemoveCurrent
	TEST_EQUALS(final, listRemoveCurrent(badList), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listRemoveCurrent(emptyList), LIST_INVALID_CURRENT);
	TEST_EQUALS(final, listRemoveCurrent(copy), LIST_SUCCESS);
	// list should be [1-> 2-> 3-> 4-> 5-> ]
	// copy should be [1-> 2-> ]


	// listSort
	TEST_EQUALS(final, listSort(badList, compareInt), LIST_NULL_ARGUMENT); // needs big checking
	// TEST_EQUALS(final, listSort(badList, &compareInt), LIST_OUT_OF_MEMORY); ? // needs big checking
	TEST_EQUALS(final, listSort(list, compareInt), LIST_SUCCESS);
	// list should be [1-> 2-> 3-> 4-> 5-> ]
	TEST_EQUALS(final, listInsertLast(list, (ListElement*)-100), LIST_SUCCESS);
	TEST_EQUALS(final, listInsertFirst(list, (ListElement*)100), LIST_SUCCESS);
	// list should be [100-> 1-> 2-> 3-> 4-> 5-> -100-> ]
	TEST_EQUALS(final, listSort(list, &compareInt), LIST_SUCCESS);
	// list should be [-100-> 100-> 1-> 2-> 3-> 4-> 5-> -100-> ]

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
