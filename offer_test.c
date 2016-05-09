/*
 * offer_test.c
 *
 *  Created on: 7 במאי 2016
 *      Author: Liron
 */

#include "Offer.h"

/*
 * WORKS WITH NEWEST VERSIONS
 */


int main(){

	// -------------<offerCreate>-------------
	Offer offer = offerCreate(5, 100000, "housing");
	// -------------</offerCreate>-------------

	// -------------<offerCopy>-------------
	Offer copy = offerCopy(offer);
	// -------------</offerCopy>-------------

	// -------------<offerDestroy>-------------
	offerDestroy(offer);
	offerDestroy(copy);
	// -------------</offerDestroy>-------------
	return 0;
}
