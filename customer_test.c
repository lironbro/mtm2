/*
 * main.c
 *
 *  Created on: 6 במאי 2016
 *      Author: Liron
 */
#include "customer.h"


#define TEST_EQUALS(res_arg, a,b) 	if (res_arg && (a != b)) { \
										res_arg = false; \
									}



#define RUN_TEST(name)  printf("Running "); \
						printf(#name);		\
						printf("... ");		\
						if (!name()) { \
							printf("[FAILED]\n");		\
							return false; \
						}								\
						printf("[SUCCESS]\n");

bool customerTest(){
	// -------------<customerCreate>-------------
	bool final = true;
	int max = 100000000, id = 1, apartmentPrice = 1315500;
	TEST_EQUALS(final, customerCreate(0, 50, 100000), NULL);
	TEST_EQUALS(final, customerCreate(50, 0, 100000), NULL);
	TEST_EQUALS(final, customerCreate(50, 4, 0), NULL);
	Customer customer = customerCreate(1, 1, max);
	// -------------</customerCreate>-------------


	// -------------<customerCopy>-------------
	Customer copy = customerCopy(customer);
	// -------------</customerCopy>-------------


	// -------------<customerPurchase>-------------
	Realtor realtor = realtorCreate("capital pigs", 7);
	realtorAddService(realtor, "housing", 5);
	char* matrix = "weewwwewwewwweee";

	TEST_EQUALS(final,
				customerPurchase(customer, realtor, "no service", 1),
					CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final,
			customerPurchase(customer, realtor, "housing", 2),
					CUSTOMER_APARTMENT_DOES_NOT_EXIST);

	realtorAddApartment(realtor, "housing", 5, max+1, 4, 4, matrix);

	TEST_EQUALS(final, customerPurchase(customer, realtor, "housing", 5),
				CUSTOMER_PURCHASE_WRONG_PROPERTIES);

	realtorAddApartment(realtor, "housing", id, apartmentPrice, 4, 4, matrix);

	TEST_EQUALS(final, customerPurchase(customer, realtor, "housing", 1),
					CUSTOMER_SUCCESS);

	// -------------</customerPurchase>-------------


	// -------------<customerMakeOffer>-------------
	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com",id, apartmentPrice),
			CUSTOMER_APARTMENT_DOES_NOT_EXIST);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "no service",
			"customer@g.com", id, apartmentPrice),
			CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", id, apartmentPrice),
			CUSTOMER_APARTMENT_DOES_NOT_EXIST);
	//                                           VVV BAD? VVV
	int badIndex = 2;
	realtorAddApartment(realtor, "housing", badIndex, max+1, 4, 4, matrix);
	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", badIndex, max+1),CUSTOMER_REQUEST_WRONG_PROPERTIES);

	realtorAddApartment(realtor, "housing", id, apartmentPrice, 4, 4, matrix);
	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", id, apartmentPrice+1),
			CUSTOMER_REQUEST_ILLOGICAL_PRICE);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", id, apartmentPrice), CUSTOMER_SUCCESS);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", id, apartmentPrice),
			CUSTOMER_ALREADY_REQUESTED);


	// -------------</customerMakeOffer>-------------


	// -------------<customerDestroy>-------------
	customerDestroy(customer);
	customerDestroy(copy);
	// -------------</customerDestroy>-------------

	realtorDestroy(realtor);

	return final;
}

int main10(){
	RUN_TEST(customerTest);
	return 0;
}
