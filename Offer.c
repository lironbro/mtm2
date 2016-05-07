/*
 * Offer.c
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#include "Offer.h"

// ------------------- <General static functions> -------------------

/*static int str_length(char* str){
	if(str == NULL)
		return -1;
	int count = 0;
	while(*str){
		str++;
		count++;
	}
	return count;
}*/

// Needs to be deleted? unnecessary function.
/*static char* str_copy(char* source, char* destination){
	free(destination);	// not sure if this is legal
	destination = malloc(sizeof(char)*(1+str_length(source)));
	if(destination == NULL){
		return NULL;
	}
	int i = 0;
	while(*(source+i)){
		*(destination+i) = *(source+i);
		i++;
	}
	*(destination+i) = *(source+i);	// places the '/0' at the end, needs checking
	return destination;
}*/





// ------------------- </General static functions> -------------------


// ------------------- <Offer> -------------------

struct offer_t{
	int id;
	int price;
	char* service_name;
};

Offer offerCreate(int id, int price, char* service_name){
	Offer offer = malloc(sizeof(*offer));
	offer->id = id;
	offer->price = price;
	int len = strlen(service_name);
	offer->service_name=malloc(len+1);
	memset(offer->service_name, '\0', len+1);
	strcpy(offer->service_name, service_name);
	//offer->service_name = strdup(service_name);
	return offer;
}

Offer offerCopy(Offer offer){
	return offerCreate(offer->id, offer->price, offer->service_name);
}

void offerDestroy(Offer offer){
	free(offer->service_name);
	free(offer);
}

int offerGetId(Offer offer){
	return offer->id;
}

int offerGetPrice(Offer offer){
	return offer->price;
}

char* offerGetServiceName(Offer offer){
	return offer->service_name;
}


// ------------------- </Offer> -------------------



