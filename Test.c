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
	char* name2="OKAY";

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
	TEST_EQUALS(final, realtorAddService(realtor1, "a_first", 5), REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS);
	TEST_EQUALS(final, realtorAddService(realtor2, "b_first", 5), REALTOR_SUCCESS);


		// REALTOR ADD APARTMENT //
	char* matrix="weewwwewwewwweee";
	/*apartment should be:
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


		// REALTOR IS REALTOR RELEVANT //



		// REALTOR COPY //
	Realtor copy = realtorCopy(realtor1);
	TEST_NOT_EQUALS(final, copy, NULL);
	TEST_EQUALS(final, realtorCopy(NULL), NULL);


		// REALTOR DESTROY //
	realtorDestroy(realtor1);
	realtorDestroy(realtor2);


	return final;
}


int main()
{
	RUN_TEST(firstTest);

	return 0;
}
