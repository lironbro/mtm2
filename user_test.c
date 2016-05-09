/*
 * user_test.c
 *
 *  Created on: 8 במאי 2016
 *      Author: Liron
 */

/*
 * WORKS WITH NEWEST VERSION OF USER.C AND USER.H
 *
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

#define TEST_RESULT(name, result)  printf("\nRunning "); \
		printf(#name);		\
		printf("... ");		\
		if (!result) { \
			printf("[FAILED]\n");		\
			return false; \
		}								\
		printf("[SUCCESS]\n");

static bool userRealtorAddServiceTest(User userRealtor, User userCustomer){
	bool final = true;

	TEST_EQUALS(final, userRealtorAddService(userRealtor, "housing", 4),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddService(userRealtor, "deleting", 4),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorAddService(userRealtor, "housing", 10),
			USER_APARTMENT_SERVICE_ALREADY_EXISTS);
	TEST_EQUALS(final, userRealtorAddService(userCustomer, "housing", 10),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);

	return final;
}

static bool userRealtorRemoveServiceTest(User userRealtor, User userCustomer){
	bool final = true;

	TEST_EQUALS(final, userRealtorRemoveService(userRealtor, "deleting"),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorRemoveService(userRealtor, "deleting"),
			USER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, userRealtorRemoveService(userCustomer, "deleting"),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);

	return final;
}

static bool userRealtorAddApartmentTest(User userRealtor, User userCustomer,
		int id, int price, int width, int height, char* matrix){
	bool final = true;

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

	return final;
}

static bool userRealtorRemoveApartmentTest(User userCustomer, User userRealtor,
		int id){
	bool final = true;

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

	return final;
}

static bool userCustomerPurchaseTest(User userCustomer, User userRealtor,
		int id){
	bool final = true;

	TEST_EQUALS(final, userCustomerPurchase(userCustomer, userCustomer,
			"housing", id), USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userCustomerPurchase(userRealtor, userRealtor,
			"housing", id), USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userCustomerPurchase(userCustomer, userRealtor,
			"no service", id), USER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, userCustomerPurchase(userCustomer, userRealtor,
			"housing", id-1), USER_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, userCustomerPurchase(userCustomer, userRealtor,
			"housing", id+1), USER_SUCCESS);
	TEST_EQUALS(final, userCustomerPurchase(userCustomer, userRealtor,
			"housing", id+1), USER_APARTMENT_DOES_NOT_EXIST);

	return final;
}

static bool userCustomerMakeOfferTest(User userCustomer, User userRealtor,
		int id, int price, int max, int width, int height, char* matrix){
	bool final = true;

	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userCustomer,
			"mymail@gmail.com", "housing", id, price),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userCustomerMakeOffer(userRealtor, userRealtor,
			"mymail@gmail.com", "housing", id, price),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userRealtor,
			"mymail@gmail.com", "no service", id, price),
			USER_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userRealtor,
			"mymail@gmail.com", "housing", id-1, price),
			USER_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userRealtor,
			"mymail@gmail.com", "housing", id-1, price),
			USER_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, userRealtorAddApartment(userRealtor, "housing", id-1,
			max+1, width, height, matrix),
			USER_SUCCESS);
	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userRealtor,
			"mymail@gmail.com", "housing", id-1, max+1),
			USER_REQUEST_WRONG_PROPERTIES);
	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userRealtor,
			"mymail@gmail.com", "housing", id+2, price+1),
			USER_REQUEST_ILLOGICAL_PRICE);
	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userRealtor,	// <<<<
			"mymail@gmail.com", "housing", id+2, price),
			USER_SUCCESS);
	TEST_EQUALS(final, userCustomerMakeOffer(userCustomer, userRealtor,
			"mymail@gmail.com", "housing", id+2, price-1),
			USER_ALREADY_REQUESTED);

	return final;
}

static bool userRealtorRespondToOfferTest(User userRealtor, User userCustomer){
	bool final = true;

	TEST_EQUALS(final, userRealtorRespondToOffer(userRealtor, userRealtor,
			"mymail@gmail.com", accept),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userRealtorRespondToOffer(userCustomer, userCustomer,
			"mymail@gmail.com", accept),
			USER_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, userRealtorRespondToOffer(userRealtor, userCustomer,
			"NOTmymail@gmail.com", accept),
			USER_NOT_REQUESTED);
	TEST_EQUALS(final, userRealtorRespondToOffer(userRealtor, userCustomer,
			"mymail@gmail.com", accept),
			USER_SUCCESS);
	TEST_EQUALS(final, userRealtorRespondToOffer(userRealtor, userCustomer,
			"mymail@gmail.com", accept),
			USER_NOT_REQUESTED);

	return final;
}

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
	TEST_EQUALS(final, userRealtorAddServiceTest(userRealtor, userCustomer),
			true);
	TEST_RESULT(yad3serviceAddRealtorTest, final);
	// -------------</userRealtorAddService>-------------



	// -------------<userRealtorRemoveService>-------------
	TEST_EQUALS(final, userRealtorRemoveServiceTest(userRealtor, userCustomer),
			true);
	TEST_RESULT(userRealtorRemoveServiceTest, final);
	// -------------</userRealtorRemoveService>-------------



	// -------------<userRealtorAddApartment>-------------
	int id = 10, price = 15000, width =4, height = 4;
	char* matrix = "weewwwewwewwweee";

	TEST_EQUALS(final, userRealtorAddApartmentTest(userRealtor, userCustomer,
			id, price, width, height, matrix),
			true);
	TEST_RESULT(userRealtorAddApartmentTest, final);

	// -------------</userRealtorAddApartment>-------------



	// -------------<userRealtorRemoveApartment>-------------
	TEST_EQUALS(final, userRealtorRemoveApartmentTest(userCustomer,
			userRealtor, id), true);
	TEST_RESULT(userRealtorRemoveApartmentTest, final);
	// -------------</userRealtorRemoveApartment>-------------



	// -------------<userCustomerPurchase>-------------
	TEST_EQUALS(final, userCustomerPurchaseTest(userCustomer,
			userRealtor, id), true);
	TEST_RESULT(userCustomerPurchaseTest, final);
	// -------------</userCustomerPurchase>-------------



	// -------------<userCustomerMakeOffer>-------------
	TEST_EQUALS(final, userCustomerMakeOfferTest(userCustomer,
			userRealtor, id, price, max, width, height, matrix), true);
	TEST_RESULT(userCustomerMakeOfferTest, final);
	// -------------</userCustomerMakeOffer>-------------



	// -------------<userRealtorRespondToOffer>-------------
	TEST_EQUALS(final, userRealtorRespondToOfferTest(userRealtor, userCustomer),
			true);
	TEST_RESULT(userRealtorRespondToOfferTest, final);
	// -------------</userRealtorRespondToOffer>-------------



	// -------------<userRealtorDestroy>-------------
	TEST_EQUALS(final, userRealtorDestroy(userRealtor), USER_SUCCESS);
	// -------------</userRealtorDestroy>-------------



	// -------------<userCustomerDestroy>-------------
	TEST_EQUALS(final, userCustomerDestroy(userCustomer), USER_SUCCESS);
	// -------------</userCustomerDestroy>-------------

	return final;
}


int main2000(){
	RUN_TEST(userTest);
	return 0;

}

