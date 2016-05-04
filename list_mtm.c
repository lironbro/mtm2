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
	Node tmpPrev = node1->previous, tmpNext = node1;
	node1->previous = node2;
	node1->next = node2->next;
	node2->previous = tmpPrev;
	node2->next = tmpNext;
	tmpPrev = NULL;
	tmpNext = NULL;
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
	free(node);
	node->info = NULL;
}

static void nodeDestroyChain(Node node, FreeListElement freeElement){
	if(node == NULL){
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
	if(list->current == NULL){
		return LIST_INVALID_CURRENT;
	}
	// if list has at least 2 nodes and we attempt to remove the first one
	if(list->current == list->first){	// we need to make sure list->first
		list->first = list->first->next; // won't be NULL- this does just that
		list->first->previous = NULL;
	}
	else{
	// erases the gap created by deleting the current node
		printf("\n............................\n");
		printf("current:: %d", *(int*)listGetCurrent(list));
		listPrint(list);
		nodeSetNext(list->current->previous, list->current->next);
		nodeSetPrevious(list->current->next, list->current->previous);
		listPrint(list);
		printf("\n............................\n");
	}
	nodeDestroy(list->current, list->freeElement);
	list->current = NULL;
	return LIST_SUCCESS;
}

static void bubble(List list, CompareListElements compareElement){ // part of bubble-sort
	if(list == NULL || list->current == NULL){
		return;
	}
	for(Node iterator = list->current; iterator->next != NULL;){
		if(compareElement(iterator->info, iterator->next->info) < 0){
			if(iterator == list->first){
				list->first=iterator->next;
			}
			nodeSwitch(iterator, iterator->next);
		}
		else {
			iterator=iterator->next;
		}
	}
}

// finds the index of current in the list, without changing it
static int getCurrentIndex(List list){
	Node original_current = listGetCurrent(list);
	int index = 0;
	while(listGetCurrent(list) != original_current){
		index++;
		listGetNext(list);
	}
	return index;
}

// we were supposed to return LIST_OUT_OF_MEMORY somewhere here, not sure where
ListResult listSort(List list, CompareListElements compareElement){
	if(list == NULL || compareElement == NULL){
		return LIST_NULL_ARGUMENT;
	}
	int original_index = getCurrentIndex(list);
	for(listGetFirst(list); list->current != NULL; listGetNext(list)){
		bubble(list, compareElement);
	}
	for(listGetFirst(list); original_index != 0; original_index--){
		listGetNext(list);
	}
	return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
	if(list == NULL || filterElement == NULL || key == NULL){
		return NULL;
	}
	List filtered = listCreate(list->copyElement, list->freeElement);
	for(listGetFirst(list); list->current != NULL; listGetNext(list)){
		if(filterElement(nodeGetInfo(list->current), key)){
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
	nodeDestroyChain(list->first, list->freeElement);
	list->first = NULL;
	list->current = NULL;
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
	if(list == NULL)
		return;
	Node original_current = list->current;
	printf(">>>> [");
	for(listGetFirst(list); list->current != NULL; listGetNext(list)){
		printf("%d ->", *(int*)(list->current->info));
	}
	printf(" ]\n");
	list->current = original_current;
}

