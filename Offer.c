/*
 * Offer.c
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#include "Offer.h"
#include <String.h>

// ------------------- <General static functions> -------------------



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
	offer->service_name = strdup(service_name);
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



