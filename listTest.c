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

void freeInt(ListElement element){	// this might be causing trouble
	free(element);
}

int compareInt(ListElement num1, ListElement num2){
	return ((*(int*)num2)-(*(int*)num1));
}

bool filterInt(ListElement num, ListFilterKey key){
	return *(int*)num >= *(int*)key;
}

bool listTest(){

	bool final=true;

	// listCreate
	// changed copyInt and freeInt to be just like in the tutorial. should work perfectly.
	List badList = listCreate(copyInt, NULL);
	TEST_EQUALS(final, badList, NULL);
	badList = listCreate(NULL, freeInt);
	TEST_EQUALS(final, badList, NULL);
	List list = listCreate(copyInt, freeInt);
	List emptyList = listCreate(copyInt, freeInt);
	printf("\n\n\n>>>>>listCreate<<<<<\n");
	printf("list should be [ ]\n");
	listPrint(list);
	// list should be [ ]

	// listInsertFirst
	int num=1;
	TEST_EQUALS(final, listInsertFirst(badList, &num), LIST_NULL_ARGUMENT);
	// TEST_EQUALS(final, listInsertFirst(list, 9999), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, listInsertFirst(list, &num), LIST_SUCCESS);
	// list should be [1-> ]
	printf("\n\n\n>>>>>listInsertFirst<<<<<\n");
	printf("list should be [1-> ]\n");
	listPrint(list);

	// listInsertLast
	TEST_EQUALS(final, listInsertLast(badList, &num), LIST_NULL_ARGUMENT);
	// TEST_EQUALS(final, listInsertLast(list, 9999), LIST_OUT_OF_MEMORY); ?
	num=3;
	TEST_EQUALS(final, listInsertLast(list, &num), LIST_SUCCESS);
	// list should be [1-> 3-> ]
	printf("\n\n\n>>>>>listInsertLast<<<<<\n");
	printf("list should be [1-> 3-> ]\n");
	listPrint(list);


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
	printf("\n\n>>>>>listCopy<<<<<\n");
	printf("list should be [1-> 3-> ]\n");
	listPrint(list);
	printf("copy should be [1-> 3-> ]\n");
	listPrint(copy);

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
	printf("\n\n>>>>>listGetNext<<<<<\n");
	printf("list should be [1-> 3-> 4-> ]\n");
	listPrint(list);
	// copy should be [1-> 3-> ]
	printf("copy should be [1-> 3-> ]\n");
	listPrint(copy);


	// listGetCurrent
	num=3;
	TEST_EQUALS(final, listGetCurrent(badList), NULL);
	TEST_EQUALS(final, listGetCurrent(emptyList), NULL);
	TEST_EQUALS(final, *(int*)(listGetCurrent(copy)), num);
	num=4;
	TEST_EQUALS(final, *(int*)(listGetCurrent(list)), num);
	// list should be [1-> 3-> 4->]
	printf("\n\n>>>>>listGetCurrent<<<<<\n");
	printf("list should be [1-> 3-> 4-> ]\n");
	listPrint(list);
	// copy should be [1-> 3-> ]
	printf("copy should be [1-> 3-> ]\n");
	listPrint(copy);


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
	printf("\n\n>>>>>listInsertBeforeCurrent<<<<<\n");
	printf("list should be [1-> 3-> 4-> ]\n");
	listPrint(list);
	// copy should be [1-> 2-> 3-> ]
	printf("copy current: %d\n", *(int*)listGetCurrent(copy));
	listPrint(copy);

	// listInsertAfterCurrent
	num=1;
	TEST_EQUALS(final, listInsertAfterCurrent(badList, &num), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listInsertAfterCurrent(emptyList, &num), LIST_INVALID_CURRENT);
	// TEST_EQUALS(final, listInsertAfterCurrent(emptyList, 500), LIST_OUT_OF_MEMORY); ?
	TEST_EQUALS(final, *(int*)(listGetFirst(copy)), num);
	num = 20;
	TEST_EQUALS(final, listInsertAfterCurrent(copy, &num), LIST_SUCCESS);
	num=5;
	TEST_EQUALS(final, listInsertAfterCurrent(list, &num), LIST_SUCCESS);
	// list should be [1-> 3-> 4-> 5-> ]
	printf("\n\n>>>>>listInsertAfterCurrent<<<<<\n");
	printf("list should be [1-> 3-> 4-> 5->]\n");
	listPrint(list);

	TEST_EQUALS(final, *(int*)(listGetNext(copy)), 20);
	TEST_EQUALS(final, *(int*)(listGetNext(copy)), 2);
	TEST_EQUALS(final, *(int*)(listGetNext(copy)), 3);
	num = 40;
	TEST_EQUALS(final, listInsertAfterCurrent(copy, &num), LIST_SUCCESS);
	// copy should be [1-> 20-> 2-> 3-> 40-> ]
	printf("copy should be [1-> 20-> 2-> ||3||-> 40-> ]\n");
	listPrint(copy);

	// listRemoveCurrent
	printf("\n\n>>>>>listRemoveCurrent<<<<<\n");
	TEST_EQUALS(final, listRemoveCurrent(badList), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listRemoveCurrent(emptyList), LIST_INVALID_CURRENT);
	TEST_EQUALS(final, listRemoveCurrent(copy), LIST_SUCCESS);
	// copy should be [1-> 20-> 2-> 40-> ]\n
	num = -666;
	listGetFirst(copy);
	// copy should be [||1||-> 20-> 2-> 40-> ]
	TEST_EQUALS(final, listInsertAfterCurrent(copy, &num), LIST_SUCCESS);
	// copy should be [||1||-> -666-> 20-> 2-> 40-> ]
	TEST_EQUALS(final, listRemoveCurrent(copy), LIST_SUCCESS);
	// copy should be [-666-> 20-> 2-> 40-> ]
	TEST_EQUALS(final, listInsertLast(copy, &num), LIST_SUCCESS);
	// copy should be [-666-> 20-> 2-> 40-> -666->]
	printf("copy should be [-666-> 20-> 2-> 40-> -666->]\n");
	listPrint(copy);
	TEST_EQUALS(final, listGetCurrent(copy), NULL);



	// listSort
	printf("\n\n>>>>>listSort<<<<<\n");
	num=-100;
	TEST_EQUALS(final, listSort(badList,
			(CompareListElements)compareInt), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listSort(copy, NULL), LIST_NULL_ARGUMENT);
	printf("list should be [1-> 3-> 4-> 5->]\n");
	listPrint(list);
	TEST_EQUALS(final, listInsertLast(list, &num), LIST_SUCCESS);
	num= 100;
	TEST_EQUALS(final, listInsertFirst(list, &num), LIST_SUCCESS);
	printf("list should be [100-> 1-> 3-> 4-> 5-> -100-> ]\n");
	listPrint(list);
	TEST_EQUALS(final, listSort(list, compareInt), LIST_SUCCESS);
	// list should be [-100-> 1-> 3-> 4-> 5-> 100->]
	printf("list should be [-100-> 1-> 3-> 4-> 5-> 100->]\n");
	listPrint(list);

	// copy should be [-666-> 20-> 2-> 40-> -666]
	num= 100;
	TEST_EQUALS(final, listInsertFirst(copy, &num), LIST_SUCCESS);
	num= 56;
	TEST_EQUALS(final, listInsertFirst(copy, &num), LIST_SUCCESS);
	num= -13;
	TEST_EQUALS(final, listInsertFirst(copy, &num), LIST_SUCCESS);
	num= -56;
	TEST_EQUALS(final, listInsertLast(copy, &num), LIST_SUCCESS);
	printf("copy should be [13-> 56-> 100-> -666->"
			" 20-> 2-> 40-> -666-> -56->]\n");
	listPrint(copy);
	TEST_EQUALS(final, listSort(copy, compareInt), LIST_SUCCESS);
	// copy should be [-666-> -666-> -56-> -13-> 2-> 20-> 40-> 56-> 100]
	printf("copy should be [-666-> -666-> -56-> "
			"-13-> 2-> 20-> 40-> 56-> 100]\n");
	listPrint(copy);


	// listFilter
	printf("\n\n>>>>>listFilter<<<<<\n");
	int key = 3;
	List filtered = listFilter(badList, filterInt, (ListFilterKey*)&key);
	// filtered should be NULL
	printf("filtered should be NULL\n");
	listPrint(filtered);
	filtered = listFilter(list, NULL, (ListFilterKey*)&key);
	// filtered should be NULL
	printf("filtered should be NULL\n");
	listPrint(filtered);
	filtered = listFilter(list, filterInt, NULL);
	// filtered should be NULL
	printf("filtered should be NULL\n");
	listPrint(filtered);
	filtered = listFilter(copy, filterInt, (ListFilterKey*)&key);
	// filtered should be [20-> 40-> 56-> 100-> ]
	printf("filtered should be [20-> 40-> 56-> 100]\n");
	listPrint(filtered);

	// listClear
	printf("\n\n>>>>>listClear<<<<<\n");
	TEST_EQUALS(final, listClear(NULL), LIST_NULL_ARGUMENT);
	TEST_EQUALS(final, listClear(copy), LIST_SUCCESS);
	// copy should be []
	printf("copy should be []");
	listPrint(copy);

	// listDestroy
	printf("\n\n>>>>>listDestroy<<<<<\n");
	listDestroy(list);
	listDestroy(copy);
	listDestroy(filtered);
	listDestroy(badList);
	listDestroy(emptyList);
	printf("list should be NULL");
	listPrint(list);
	printf("copy should be NULL");
	listPrint(copy);
	printf("filtered should be NULL");
	listPrint(filtered);
	printf("badList should be NULL");
	listPrint(badList);
	printf("emptyList should be NULL");
	listPrint(emptyList);

	return final;

}



int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(listTest);
	return 0;
}
