/*
 * list_mtm.c
 *
 *  Created on: 11 באפר׳ 2016
 *      Author: Liron
 */

#include "list_mtm.h"
#include <stdio.h>
#include <stdlib.h>




// ---------------------------------------------- NODE

typedef struct Node_t* Node;


struct Node_t{
	ListElement info;
	Node next;
	Node previous;
};


// creates a new node
Node nodeCreate(ListElement element,
		Node next, Node previous, CopyListElement copyElement){
	Node node = malloc(sizeof(*node));	// crashes
	if(node == NULL){
		return NULL;
	}
	node->info = copyElement(element);
	node->next = next;
	node->previous = previous;
	return node;
}

/*

// copies the contents of the node into a new node
static Node nodeCopy(Node node, CopyListElement copyElement){
	if(node == NULL){
		return NULL;
	}
	return nodeCreate(copyElement(node->info), node->next, node->previous, copyElement);
}

*/

// switches between the nodes' next and previous pointers
static void nodeSwitch(Node node1, Node node2){
	if(node1 == NULL || node2 == NULL){
		return;
	}
	ListElement temp = node1->info;
	node1->info = node2->info;
	node2->info = temp;
}

/*

// sets the info of node to be a copy of the given element
static void nodeSetInfo(Node node, ListElement element, CopyListElement copyElement){
	if(node == NULL || element == NULL){
		return;
	}
	node->info = copyElement(element);
}

*/

// sets the next node for "node" to be "next"
static void nodeSetNext(Node node, Node next){
	if(node == NULL){
		return;
	}
	node->next = next;
}


// sets the previous node for "node" to be "previous"
static void nodeSetPrevious(Node node, Node previous){
	if(node == NULL){
		return;
	}
	node->previous = previous;
}


// returns the info in node, but not as a copy
static ListElement nodeGetInfo(Node node){
	if(node == NULL){
		return NULL;
	}
	return node->info;
}


// returns the next node for "node"
static Node nodeGetNext(Node node){
	if(node == NULL){
		return NULL;
	}
	return node->next;
}


// returns the previous node for "node"
static Node nodeGetPrevious(Node node){
	if(node == NULL){
		return NULL;
	}
	return node->previous;
}


// destroys the current node only
static void nodeDestroy(Node node, FreeListElement freeElement){
	if(node == NULL){
		return;
	}
	freeElement(node->info);
	node->info = NULL;
	node->next = NULL;
	node->previous = NULL;
	free(node);

}

// destroys the following nodes and then the current one
static void nodeDestroyChain(Node node, FreeListElement freeElement){
	if(node == NULL || freeElement == NULL){
		return;
	}
	nodeDestroyChain(node->next, freeElement);
	nodeDestroy(node, freeElement);
}

// ---------------------------------------------- LIST


struct List_t{
	Node first;
	Node current;
	CopyListElement copyElement;
	FreeListElement freeElement;
};

// --------------------<Static functions>------------------------

// finds the index of current in the list, without changing it
int listGetCurrentIndex(List list){
	Node original_current = list->current;
	listGetFirst(list);
	int index = 0;
	while(list->current != original_current){
		index++;
		listGetNext(list);
	}
	return index;
}

static void listSetCurrentToIndex(List list, int index){
	if(list == NULL || index <0 || listGetFirst(list) == NULL)
		return;
	for(listGetFirst(list); index != 0; index--){
		listGetNext(list);
	}
}

// --------------------</Static functions>------------------------


List listCreate(CopyListElement copyElement, FreeListElement freeElement){
	List list = malloc(sizeof(*list));	// <<<<<<crashes here in listFilter
	if(list == NULL || copyElement == NULL || freeElement == NULL){
		free(list);
		return NULL;
	}
	list->copyElement = copyElement;
	list->freeElement = freeElement;
	list->first = NULL;
	list->current = NULL;
	return list;
}

List listCopy(List list){
	if(list == NULL || list->copyElement == NULL || list->freeElement == NULL){
		return NULL;
	}
	List copy = listCreate(list->copyElement, list->freeElement);
	if(copy == NULL){
		return NULL;
	}
	//copy->copyElement = list->copyElement;
	//copy->freeElement = list->freeElement;
	Node original_current = list->current, copy_current;
	for(listGetFirst(list); list->current != NULL;
			listGetNext(list), listGetNext(copy)){
		listInsertLast(copy, nodeGetInfo(list->current));
		if(list->current == original_current)
			copy_current = copy->current;
	}
	list->current = original_current;
	copy->current = copy_current;
	return copy;
}

int listGetSize(List list){
	if(list == NULL){
		return -1;
	}
	Node original_current = list->current;
	int length = 0;
	for(listGetFirst(list); list->current != NULL; listGetNext(list)){
		length++;
	}
	list->current = original_current;
	return length;
}

ListElement listGetFirst(List list){
	if(list == NULL || list->first == NULL){
		return NULL;
	}
	list->current = list->first;
	return nodeGetInfo(list->current);
}

ListElement listGetNext(List list){
	if(list == NULL || list->current == NULL){
		return NULL;
	}
	list->current = nodeGetNext(list->current);
	if(list->current == NULL)
		return NULL;
	return nodeGetInfo(list->current);
}

ListElement listGetCurrent(List list){
	if(list == NULL || list->current == NULL){
		return NULL;
	}
	return nodeGetInfo(list->current);
}


ListResult listInsertFirst(List list, ListElement element){
	if(list == NULL){
		return LIST_NULL_ARGUMENT;
	}
	Node node = nodeCreate(element, list->first, NULL, list->copyElement);
	if(node == NULL){
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetPrevious(list->first, node);
	list->first = node;
	return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element){
	if(list == NULL){
		return LIST_NULL_ARGUMENT;
	}
	if(list->first == NULL){	// if the list is empty this places the node at the start
		return listInsertFirst(list, element);
	}
	Node node = nodeCreate(element, NULL, NULL, list->copyElement);
	if(node == NULL){
		return LIST_OUT_OF_MEMORY;
	}
	Node original_current = list->current;
	for(listGetFirst(list); nodeGetNext(list->current) != NULL; listGetNext(list));
	nodeSetPrevious(node, list->current);
	nodeSetNext(list->current, node);
	list->current = original_current;
	return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element){
	if(list == NULL){
		return LIST_NULL_ARGUMENT;
	}
	if(list->current == NULL){
		return LIST_INVALID_CURRENT;
	}
	Node node = nodeCreate(element, list->current,
			nodeGetPrevious(list->current), list->copyElement);
	if(node == NULL){
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetNext(nodeGetPrevious(list->current), node);	// sets the new node between
	nodeSetPrevious(list->current, node);	// the previous node and the current one
	return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element){
	if(list == NULL){
		return LIST_NULL_ARGUMENT;
	}
	if(list->current == NULL){
		return LIST_INVALID_CURRENT;
	}
	Node node = nodeCreate(element,
			nodeGetNext(list->current), list->current, list->copyElement);
	if(node == NULL){
		return LIST_OUT_OF_MEMORY;
	}
	nodeSetPrevious(nodeGetNext(list->current), node); // node and the Current one
	nodeSetNext(list->current, node);	// sets the new node between the previous
	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list){	// <<<<< big leak here
	if(list == NULL){
		return LIST_NULL_ARGUMENT;
	}
	if(list->current == NULL || list->first == NULL){
		return LIST_INVALID_CURRENT;
	}
	// if list has at least 2 nodes and we attempt to remove the first one
	if(list->current == list->first){	// we need to make sure list->first
		list->first = list->first->next; // won't be NULL- this does just that
		if(list->first != NULL && list->first->previous != NULL)
			list->first->previous = NULL;
	}
	else{
	// erases the gap created by deleting the current node
		nodeSetNext(list->current->previous, list->current->next);
		nodeSetPrevious(list->current->next, list->current->previous);
	}
	nodeDestroy(list->current, list->freeElement);	// <<<<< PROBLEM HERE
	list->current = NULL;
	return LIST_SUCCESS;
}



int bubble(List list, CompareListElements compareElement){ // part of bubble-sort
	if(list == NULL || list->first == NULL){
		return -1;
	}
	int original_index = listGetCurrentIndex(list), switched = 0;
	for(listGetFirst(list); list->current->next != NULL; listGetNext(list)){
		if(compareElement(list->current->info, list->current->next->info) < 0){
			nodeSwitch(list->current, list->current->next);
			switched = 1;
		}
	}
	listSetCurrentToIndex(list, original_index);
	return switched;
}



// we were supposed to return LIST_OUT_OF_MEMORY somewhere here, not sure where
ListResult listSort(List list, CompareListElements compareElement){
	if(list == NULL || compareElement == NULL){
		return LIST_NULL_ARGUMENT;
	}
	int original_index = listGetCurrentIndex(list);	// current's original index
	for(listGetFirst(list); list->current != NULL; listGetNext(list)){
		if(bubble(list, compareElement) == 0)
			break;
	}
	listSetCurrentToIndex(list, original_index);
	return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
	if(list == NULL || filterElement == NULL || key == NULL){
		return NULL;
	}
	List filtered = listCreate(list->copyElement, list->freeElement);
	for(listGetFirst(list); list->current != NULL; listGetNext(list)){
		if(filterElement(nodeGetInfo(list->current), key) == true){
			listInsertLast(filtered, nodeGetInfo(list->current));
		}
	}
	return filtered;
}

ListResult listClear(List list)
{
	if(list == NULL){
		return LIST_NULL_ARGUMENT;
	}
	for(listGetFirst(list); list->first != NULL; listRemoveCurrent(list)){
		listGetFirst(list);
	}
	return LIST_SUCCESS;
}

void listDestroy(List list)
{
	if(list == NULL){
		return;
	}
	listClear(list);
	free(list);
}

void listPrint(List list){
	if(list == NULL){
		printf(">>>> list is NULL\n");
		return;
	}
	Node original_current = list->current;
	printf(">>>> [");
	for(listGetFirst(list); list->current != NULL; listGetNext(list)){
		if(list->current == original_current)
			printf("||%d|| ->", *(int*)(list->current->info));
		else
			printf("%d ->", *(int*)(list->current->info));
	}
	printf(" ]\n");
	list->current = original_current;
}

