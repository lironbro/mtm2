/*
 * user_test.c
 *
 *  Created on: 8 במאי 2016
 *      Author: Liron
 */


#include "User.h"

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

bool userTest(){
	bool final = true;

	// -------------<userRealtorCreate>-------------
	int tax = 10;
	TEST_EQUALS(final, userRealtorCreate(NULL, tax), NULL);
	TEST_EQUALS(final, userRealtorCreate("capital pig", -tax), NULL);
	User userRealtor = userRealtorCreate("capital pig", tax);
	// -------------</userRealtorCreate>-------------



	// -------------<userCustomerCreate>-------------
	int max = 1000000;
	TEST_EQUALS(final, userCustomerCreate(0, 1, max), NULL);
	TEST_EQUALS(final, userCustomerCreate(1, 0, max), NULL);
	TEST_EQUALS(final, userCustomerCreate(1, 1, 0), NULL);
	User userCustomer = userCustomerCreate(1, 1, max);

	// -------------</userCustomerCreate>-------------



	// -------------<userRealtorAddService>-------------
	TEST_EQUALS(final, userRealtorAddService(userRealtor, "housing", 4),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddService(userRealtor, "deleting", 4),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddService(userRealtor, "housing", 10),
			USER_APARTMENT_SERVICE_ALREADY_EXISTS);
	TEST_EQUALS(final, userRealtorAddService(userCustomer, "housing", 10),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);
	// -------------</userRealtorAddService>-------------



	// -------------<userRealtorRemoveService>-------------
	TEST_EQUALS(final, userRealtorRemoveService(userRealtor, "deleting"),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorRemoveService(userRealtor, "deleting"),
			USER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, userRealtorRemoveService(userCustomer, "deleting"),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);
	// -------------</userRealtorRemoveService>-------------



	// -------------<userRealtorAddApartment>-------------
	int id = 10, price = 15000, width =4, height = 4;
	char* matrix = "weewwwewwewwweee";
	TEST_EQUALS(final, userRealtorAddApartment(userCustomer, "housing", id,
			price, width, height, matrix), USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "deleting", id,
			price, width, height, matrix),
			USER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "housing", id,
			price, width, height, matrix),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "housing", id,
			price, width, height, matrix),
			USER_APARTMENT_ALREADY_EXISTS);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "housing", id+1,
			price, width, height, matrix),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "housing", id+2,
			price, width, height, matrix),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "housing", id+3,
			price, width, height, matrix),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "housing", id+34,
			price, width, height, matrix),
			USER_APARTMENT_SERVICE_FULL);
	// -------------</userRealtorAddApartment>-------------



	// -------------<userRealtorRemoveApartment>-------------
	TEST_EQUALS(final, userRealtorRemoveApartment(userCustomer, "deleting", id),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userRealtorRemoveApartment(userRealtor, "deleting", id),
			USER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, userRealtorRemoveApartment(userRealtor, "housing", id-1),
			USER_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, userRealtorRemoveApartment(userRealtor, "housing", id),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorRemoveApartment(userRealtor, "housing", id),
			USER_APARTMENT_DOES_NOT_EXIST);
	// -------------</userRealtorRemoveApartment>-------------



	// -------------<userCustomerPurchase>-------------
	TEST_EQUALS(final, userCustomerPurchase(userCustomer, userCustomer,
			"housing", id), USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userCustomerPurchase(userRealtor, userRealtor,
			"housing", id), USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userCustomerPurchase(userRealtor, userRealtor,
			"no service", id), USER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, userCustomerPurchase(userRealtor, userRealtor,
			"housing", id-1), USER_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, userCustomerPurchase(userRealtor, userRealtor,
			"housing", id+1), USER_SUCCESS);
	// -------------<userCustomerPurchase>-------------



	// -------------<userRealtorDestroy>-------------
	TEST_EQUALS(final, userRealtorDestroy(userRealtor), USER_SUCCESS);
	// -------------</userRealtorDestroy>-------------



	// -------------<userCustomerDestroy>-------------
	TEST_EQUALS(final, userCustomerDestroy(userCustomer), USER_SUCCESS);
	// -------------</userCustomerDestroy>-------------

	return final;
}


int main(){
	RUN_TEST(userTest);
	return 0;

}

