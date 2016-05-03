/*
 * Offer.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#ifndef OFFER_H_
#define OFFER_H_

#include "yad3service.h"
typedef struct offer_t* Offer;

Offer offerCreate(int id, int price, char* service_name);

void offerDestroy(Offer offer);

int offerGetId(Offer offer);

int offerGetPrice(Offer offer);

char* offerGetServiceName(Offer offer);

#endif /* OFFER_H_ */
