/*
 * Offer.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#ifndef OFFER_H_
#define OFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <String.h>

typedef struct offer_t* Offer;

/*
 * offerCreate: creates a new offer, which will keep the id and price of the
 * apartment, and the service in which it is located
 *
 * @param id: the id of the apartment for which the offer is made
 * @param price: price detailed in the offer
 * @param service_name: new of the service
 */

Offer offerCreate(int id, int price, char* service_name);

/*
 * offerCopy: copies an offer
 *
 * @param offer: the offer in need of copying
 */
Offer offerCopy(Offer offer);

/*
 * offerDestroy: destroys an offer
 *
 * @param offer: the offer in need of destroying
 */
void offerDestroy(Offer offer);

/*
 * offerGetId: returns an offer's ID
 */
int offerGetId(Offer offer);

/*
 * offerGetPrice: returns an offer's price
 */
int offerGetPrice(Offer offer);

/*
 * offerGetServiceName: returns an offer service name
 */
char* offerGetServiceName(Offer offer);

#endif /* OFFER_H_ */
