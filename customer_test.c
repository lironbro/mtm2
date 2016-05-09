/*
 * main.c
 *
 *  Created on: 6 במאי 2016
 *      Author: Liron
 */
#include "customer.h"

/*
 * WORKS WITH NEWEST VERSIONS
 */



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
#define TEST_RESULT(name, result)  printf("\nRunning "); \
		printf(#name);		\
		printf("... ");		\
		if (!result) { \
			printf("[FAILED]\n");		\
			return false; \
		}								\
		printf("[SUCCESS]\n");


static bool customerPurchaseTest(Customer customer, Realtor realtor, int id,
		int width, int height, char* matrix, int max, int apartment_price){
	bool final = true;

	TEST_EQUALS(final,
			customerPurchase(customer, realtor, "no service", id),
			CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final,
			customerPurchase(customer, realtor, "housing", id),
			CUSTOMER_APARTMENT_DOES_NOT_EXIST);

	realtorAddApartment(realtor, "housing", id*2+1, max+1, width, height,
			matrix);

	TEST_EQUALS(final, customerPurchase(customer, realtor, "housing", 2*id+1),
			CUSTOMER_PURCHASE_WRONG_PROPERTIES);

	realtorAddApartment(realtor, "housing", id, apartment_price, width, height,
			matrix);

	TEST_EQUALS(final, customerPurchase(customer, realtor, "housing", id),
			CUSTOMER_SUCCESS);

	TEST_EQUALS(final, customerPurchase(customer, realtor, "housing", id),
			CUSTOMER_APARTMENT_DOES_NOT_EXIST);

	return final;
}

static bool customerMakeOfferTest(Customer customer, Realtor realtor, int id,
		int width, int height, char* matrix, int max, int apartment_price,
		int bad_index){
	bool final = true;

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com",id, apartment_price),
			CUSTOMER_APARTMENT_DOES_NOT_EXIST);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "no service",
			"customer@g.com", id, apartment_price),
			CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", id, apartment_price),
			CUSTOMER_APARTMENT_DOES_NOT_EXIST);

	realtorAddApartment(realtor, "housing", bad_index, max+1, width, height,
			matrix);
	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", bad_index, max+1),CUSTOMER_REQUEST_WRONG_PROPERTIES);

	int new_id = id+1000;
	realtorAddApartment(realtor, "housing", new_id, apartment_price, width,
			height,	matrix);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", new_id, apartment_price+1),
			CUSTOMER_REQUEST_ILLOGICAL_PRICE);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", new_id, apartment_price), CUSTOMER_SUCCESS);

	TEST_EQUALS(final, customerMakeOffer(customer, realtor, "housing",
			"customer@g.com", new_id, apartment_price),
			CUSTOMER_ALREADY_REQUESTED);

	return final;
}


bool customerTest(){
	// -------------<customerCreate>-------------
	bool final = true;
	int max = 100000000, apartment_price = 1315500;
	TEST_EQUALS(final, customerCreate(0, 50, 100000), NULL);
	TEST_EQUALS(final, customerCreate(50, 0, 100000), NULL);
	TEST_EQUALS(final, customerCreate(50, 4, 0), NULL);
	Customer customer = customerCreate(1, 1, max);
	// -------------</customerCreate>-------------


	// -------------<customerCopy>-------------
	Customer copy = customerCopy(customer);
	// -------------</customerCopy>-------------


	// -------------<customerPurchase>-------------
	char* matrix = "weewwwewwewwweee";
	int width = 4, height = 4, max_apartments = 5, id = 2, tax = 7;

	Realtor realtor = realtorCreate("capital pigs", tax);
	realtorAddService(realtor, "housing", max_apartments);

	TEST_EQUALS(final, customerPurchaseTest(customer, realtor, id, width,
			height, matrix, max, apartment_price),true);
	TEST_RESULT(customerPurchaseTest, final);
	// -------------</customerPurchase>-------------


	// -------------<customerMakeOffer>-------------
	int bad_index = 2;
	TEST_EQUALS(final, customerMakeOfferTest(customer, realtor, id, width,
			height, matrix, max, apartment_price, bad_index),true);
	TEST_RESULT(customerMakeOfferTest, final);

	// -------------</customerMakeOffer>-------------


	// -------------<customerDestroy>-------------
	customerDestroy(customer);
	customerDestroy(copy);
	// -------------</customerDestroy>-------------

	realtorDestroy(realtor);

	return final;
}

int main00(){
	RUN_TEST(customerTest);
	return 0;
}
