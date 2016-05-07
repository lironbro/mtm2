/*
 * Test.c
 *
 *  Created on: 5 במאי 2016
 *      Author: Zivkr
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "Customer.h"
#include "Realtor.h"
#include "map.h"

#define TEST_EQUALS(res_arg, a,b) 	if (res_arg && (a != b)) { \
										res_arg = false; \
									}
#define TEST_NOT_EQUALS(res_arg, a,b) 	if (res_arg && (a == b)) { \
										res_arg = false; \
									}

/*bool firstTest()
{
	char* name="YES";
	int tax_percentage=20;
	bool final;
	final = ASSERT_TEST(realtorCreate(name, tax_percentage));

	return final;
}*/

bool firstTest()
{
	bool final=true;

	int tax_percentage1=99;
	int tax_percentage2=10;
	char* name1="THIS IS SPARTA";
	char* name2="THIS IS NOT SPARTA";

		// REALTOR CREATE //
	TEST_EQUALS(final, realtorCreate(name1, -100), NULL);
	TEST_EQUALS(final, realtorCreate(name1, 101), NULL);
	TEST_EQUALS(final, realtorCreate(NULL, 10), NULL);
	TEST_EQUALS(final, realtorCreate("", 10), NULL);
	Realtor realtor1 = realtorCreate(name1, tax_percentage1);
	Realtor realtor2 = realtorCreate(name2, tax_percentage2);


		// REALTOR ADD SERVICE //
	//ASSERT_TEST(realtorAddService())
	TEST_EQUALS(final, realtorAddService(NULL, "HELLO", 10), REALTOR_NULL_ARGUMENT);
	TEST_EQUALS(final, realtorAddService(realtor1, NULL, 10), REALTOR_NULL_ARGUMENT);
	TEST_EQUALS(final, realtorAddService(realtor1, "YEAHHHHHH", -1), REALTOR_INVALID_PARAMETERS);
	TEST_EQUALS(final, realtorAddService(realtor1, "a_first", 2), REALTOR_SUCCESS);
	TEST_EQUALS(final, realtorAddService(realtor1, "a_second", 10), REALTOR_SUCCESS);
	TEST_EQUALS(final, realtorAddService(realtor1, "a_third", 50), REALTOR_SUCCESS);
	TEST_EQUALS(final, realtorAddService(realtor1, "a_first", 5),
			REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS);
	TEST_EQUALS(final, realtorAddService(realtor2, "b_first", 5), REALTOR_SUCCESS);


		// REALTOR ADD APARTMENT //
	char* matrix="weewwwewwewwweee";
	/*matrix should be:
		W E E W
		W W E W
		W E W W
		W E E E */
	int price = 1200;
	int width = 4;
	int height = 4;
	TEST_EQUALS(final, realtorAddApartment(realtor1, "a_first", -1,
			price, width, height, matrix), REALTOR_INVALID_PARAMETERS);
	TEST_EQUALS(final, realtorAddApartment(realtor1, "a_first", 2,
				price, 3, 3, matrix), REALTOR_INVALID_PARAMETERS);
	TEST_EQUALS(final, realtorAddApartment(realtor1, "a_first", 2,
				price, 5, 4, matrix), REALTOR_INVALID_PARAMETERS);
	TEST_EQUALS(final, realtorAddApartment(realtor1, "a_first", 2,
				price, -1, height, matrix), REALTOR_INVALID_PARAMETERS);
	//Adding an apartment:
	realtorResult result = realtorAddApartment(realtor1, "a_first", 1, price,
			width, height, matrix);
	TEST_EQUALS(final, result, REALTOR_SUCCESS);
	TEST_EQUALS(final, realtorAddApartment(realtor1, "a_first", 1,
				price, width, height, matrix),REALTOR_APARTMENT_ALREADY_EXISTS);
	realtorAddApartment(realtor1, "a_first", 2, price, width, height, matrix);
	TEST_EQUALS(final, realtorGetTotalApartmentNumber(realtor1), 2);
	TEST_EQUALS(final, realtorAddApartment(realtor1, "a_first", 3,
				price, width, height, matrix), REALTOR_APARTMENT_SERVICE_FULL);


		// REALTOR REMOVE APARTMENT //
	TEST_EQUALS(final, realtorRemoveApartment(NULL, "BAD_PARAM", 2),
			REALTOR_NULL_ARGUMENT);
	TEST_EQUALS(final, realtorRemoveApartment(realtor2, NULL, 2),
			REALTOR_NULL_ARGUMENT);
	TEST_EQUALS(final, realtorRemoveApartment(realtor2, "BAD_PARAM2", -1),
			REALTOR_INVALID_PARAMETERS);
	TEST_EQUALS(final, realtorRemoveApartment(realtor2, "", 3),
			REALTOR_INVALID_PARAMETERS);
	TEST_EQUALS(final, realtorRemoveApartment(realtor2, "DOESN'T EXIST!!", 3),
			REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, realtorRemoveApartment(realtor1, "a_first", 4),
			REALTOR_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, realtorRemoveApartment(realtor1, "a_first", 1),
			REALTOR_SUCCESS);
	TEST_EQUALS(final, realtorRemoveApartment(realtor1, "a_first", 2),
			REALTOR_SUCCESS);
	TEST_EQUALS(final, serviceNumberOfApatments(mapGet(realtorGetServices
			(realtor1), "a_first")), 0);
	realtorAddApartment(realtor1, "a_first", 1, price,
				width, height, matrix); // So the services won't be empty.


		// REALTOR RESPOND TO OFFER //
	Customer customer = customerCreate(5, 1, 2500);
	TEST_NOT_EQUALS(final, customer, NULL);
	TEST_EQUALS(final, customerMakeOffer(customer, realtor1, 1, 1150, "a_first",
			"dumb_customer"), CUSTOMER_SUCCESS);
	TEST_EQUALS(final, realtorRespondToOffer(realtor1, "wise_customer", accept),
			REALTOR_NOT_REQUESTED);
	TEST_EQUALS(final, realtorRespondToOffer(realtor1, "dumb_customer",
			decline), REALTOR_SUCCESS);
	TEST_EQUALS(final, customerMakeOffer(customer, realtor1, 1, 1150, "a_first",
			"dumb_customer"), CUSTOMER_SUCCESS);
	TEST_EQUALS(final, realtorRespondToOffer(realtor1, "dumb_customer",
			accept), REALTOR_SUCCESS);
	//Adding the apartment again:
	result = realtorAddApartment(realtor1, "a_first", 1, price,
				width, height, matrix);
	TEST_EQUALS(final, result, REALTOR_SUCCESS);
	customerMakeOffer(customer, realtor1, 1, 1150, "a_first","dumb_customer");

		// REALTOR IS REALTOR RELEVANT //
	TEST_EQUALS(final, realtorIsRealtorRelevant(NULL, 1, 1, 1), false);
	TEST_EQUALS(final, realtorIsRealtorRelevant(realtor1, -1, -1, -1), false);
	TEST_EQUALS(final, realtorIsRealtorRelevant(realtor2, 1, 1, 4000), false);
	result = realtorIsRealtorRelevant(realtor1,
			customerGetMinArea(customer), customerGetMinRooms(customer),
			customerGetMaxPrice(customer));
	TEST_EQUALS(final, result, true);
	TEST_EQUALS(final, realtorIsRealtorRelevant(realtor1, 20, 1, 4000), false);


		// REALTOR GET FUNCTIONS //

	TEST_EQUALS(final, strcmp(realtorGetCompanyName(realtor1),
			"THIS IS SPARTA"), 0);
	TEST_EQUALS(final, strcmp(realtorGetCompanyName(realtor2),
			"THIS IS NOT SPARTA"), 0);
	TEST_EQUALS(final, realtorGetTaxPercentage(realtor1), 99);
	TEST_EQUALS(final, realtorGetTaxPercentage(realtor2), 10);


		// REALTOR GET TOTAL APARTMENT NUMBER //
	TEST_EQUALS(final, realtorGetTotalApartmentNumber(realtor1), 1);
	realtorAddApartment(realtor1, "a_second", 1, 1000, 4, 4, matrix);
	TEST_EQUALS(final, realtorGetTotalApartmentNumber(realtor1), 2);
	TEST_EQUALS(final, realtorGetTotalApartmentNumber(realtor2), 0);
	realtorRemoveApartment(realtor1, "a_first", 1);
	TEST_EQUALS(final, realtorGetTotalApartmentNumber(realtor1), 1);
	// Now realtor1 has only 1 apartment (in "a_second")


		// REALTOR COPY //
	Realtor copy = realtorCopy(realtor1);
	TEST_NOT_EQUALS(final, copy, NULL);
	TEST_EQUALS(final, realtorCopy(NULL), NULL);


		// REALTOR GET AVERAGE MEDIAN PRICE //
	int realtor1Average=1000;
	TEST_EQUALS(final, realtorGetAverageMedianPrice(copy), realtor1Average);
	// New Realtor:
	Realtor realtor3 = realtorCreate("SPARTA COMEBACK", 70);
	realtorAddService(realtor3, "c_first", 3);
	realtorAddService(realtor3, "c_second", 4);
	realtorAddApartment(realtor3, "c_first", 1, 500, 4, 4, matrix);
	realtorAddApartment(realtor3, "c_first", 2, 800, 4, 4, matrix);
	realtorAddApartment(realtor3, "c_second", 1, 250, 4, 4, matrix);
	TEST_EQUALS(final, realtorGetAverageMedianPrice(realtor3), 300);


		// REALTOR GET AVERAGE MEDIAN AREA //
	char* matrix2="wewwewwew";
	/*matrix2 should be:
	 * 	W E W
	 * 	W E W
	 * 	W E W
	 */
	realtorAddApartment(realtor3, "c_second", 2, 900, 3, 3, matrix2);
	TEST_EQUALS(final, realtorGetAverageMedianArea(realtor3), 3);
	TEST_EQUALS(final, realtorGetAverageMedianArea(realtor1), 7);


		// REALTOR DESTROY //
	realtorDestroy(realtor1);
	realtorDestroy(realtor2);
	realtorDestroy(realtor3);
	realtorDestroy(copy);



	return final;
}


int main()
{
	RUN_TEST(firstTest);

	return 0;
}
