/*
 * offer_test.c
 *
 *  Created on: 7 במאי 2016
 *      Author: Liron
 */

#include "Offer.h"
#include "test_utilities.h"

#define TEST_EQUALS(res_arg, a,b) 	if (res_arg && (a != b)) { \
										res_arg = false; \
									}

#define TEST_NOT_EQUALS(res_arg, a,b) 	if (res_arg && (a == b)) { \
										res_arg = false; \
									}

bool Test(){

	bool final=true;
	// -------------<offerCreate>-------------
	Offer offer = offerCreate(5, 100000, "housing");
	// -------------</offerCreate>-------------

	// -------------<offerGetFunctions>-------------
	TEST_EQUALS(final, offerGetId(offer), 5);
	TEST_EQUALS(final, offerGetPrice(offer), 100000);
	TEST_EQUALS(final, strcmp(offerGetServiceName(offer), "housing"), 0);
	// -------------</offerGetFunctions>-------------

	// -------------<offerCopy>-------------
	Offer copy = offerCopy(offer);
	TEST_NOT_EQUALS(final, copy, NULL);
	// -------------</offerCopy>-------------

	// -------------<offerDestroy>-------------
	offerDestroy(offer);
	offerDestroy(copy);
	// -------------</offerDestroy>-------------

	return final;
}
int main3(){

	RUN_TEST(Test);

	return 0;
}
