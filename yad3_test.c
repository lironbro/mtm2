/*
 * yad3_test.c
 *
 *  Created on: 8 במאי 2016
 *      Author: Liron
 */

#include "yad3service.h"


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


static bool yad3serviceAddRealtorTest(yad3service yad3){
	bool final = true;
	int tax = 7;
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "bad email",
			"capital pig", 10), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "realtor@gmail.com",
			"capital pig", 0), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "realtor@gmail.com",
			"capital pig", 101), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "realtor@gmail.com",
			"capital pig", 0), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "realtor0@gmail.com",
			"capital pig", tax), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "realtor1@gmail.com",
			"capital pig", tax), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "realtor2@gmail.com",
			"capital pig", tax), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceAddRealtor(yad3, "realtor0@gmail.com",
			"capital pig", tax), YAD3_EMAIL_ALREADY_EXISTS);

	return final;
}

static bool yad3serviceAddCustomerTest(yad3service yad3, int min_area,
		int min_rooms, int max_price){
	bool final = true;

	TEST_EQUALS(final, yad3serviceAddCustomer(yad3, "bad email",
			min_area, min_rooms, max_price), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceAddCustomer(yad3, "customer0@gmail.com",
			min_area-1, min_rooms, max_price), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceAddCustomer(yad3, "customer0@gmail.com",
			min_area, min_rooms-1, max_price), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceAddCustomer(yad3, "realtor0@gmail.com",
			min_area, min_rooms, max_price), YAD3_EMAIL_ALREADY_EXISTS);
	TEST_EQUALS(final, yad3serviceAddCustomer(yad3, "customer0@gmail.com",
			min_area, min_rooms, max_price), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceAddCustomer(yad3, "customer1@gmail.com",
			min_area, min_rooms, max_price), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceAddCustomer(yad3, "customer2@gmail.com",
			min_area, min_rooms, max_price), YAD3_SUCCESS);

	return final;
}

static bool yad3serviceRemoveRealtorTest(yad3service yad3){
	bool final = true;

	TEST_EQUALS(final, yad3serviceRemoveRealtor(yad3, "noRealtor@gmail.com"),
			YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRemoveRealtor(yad3, "bad mail.com"),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRemoveRealtor(yad3, "customer1@gmail.com"),
			YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRemoveRealtor(yad3, "realtor0@gmail.com"),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRemoveRealtor(yad3, "realtor0@gmail.com"),
			YAD3_EMAIL_DOES_NOT_EXIST);

	return final;
}

static bool yad3serviceRemoveCustomerTest(yad3service yad3){
	bool final = true;

	TEST_EQUALS(final, yad3serviceRemoveCustomer(yad3, "noCustomer@gmail.com"),
			YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRemoveCustomer(yad3, "bad mail.com"),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRemoveCustomer(yad3, "realtor1@gmail.com"),
			YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRemoveCustomer(yad3, "customer0@gmail.com"),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRemoveCustomer(yad3, "customer0@gmail.com"),
			YAD3_EMAIL_DOES_NOT_EXIST);

	return final;
}

static bool yad3serviceRealtorAddApartmentServiceTest(yad3service yad3,
		int max_apartments){
	bool final = true;

	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"bad mail.com", "housing", max_apartments),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"realtor1@gmail.com", "housing", 0), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"noRealtor@gmail.com", "housing", max_apartments),
			YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"customer1@gmail.com", "housing", max_apartments),
			YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"realtor1@gmail.com", "housing", max_apartments),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"realtor2@gmail.com", "housing", max_apartments),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"realtor2@gmail.com", "to be removed", max_apartments),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentService(yad3,
			"realtor1@gmail.com", "housing", max_apartments),
			YAD3_APARTMENT_SERVICE_ALREADY_EXISTS);

	return final;
}

static bool yad3serviceRealterRemoveApartmentServiceTest(yad3service yad3){
	bool final = true;

	TEST_EQUALS(final, yad3serviceRealterRemoveApartmentService(yad3,
			"bad email.com", "housing"), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealterRemoveApartmentService(yad3,
			"noRealtor@gmail.com", "housing"), YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealterRemoveApartmentService(yad3,
			"customer1@gmail.com", "housing"), YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRealterRemoveApartmentService(yad3,
			"realtor2@gmail.com", "to be removed"),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealterRemoveApartmentService(yad3,
			"realtor2@gmail.com", "to be removed"),
			YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	return final;
}

static bool yad3serviceRealtorAddApartmentTest(yad3service yad3, int id,
		int price, int width, int height, char* matrix){
	bool final = true;

	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"bad email.com", "housing", id, price, width, height, matrix),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1", "housing", -1, price, width, height, matrix),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id, 0, width, height, matrix),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id, price, 0, height, matrix),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id, price, width, height, NULL),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"noRealtor@gmail.com", "housing", id, price, width, height, matrix),
			YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"customer1@gmail.com", "housing", id, price, width, height, matrix),
			YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "no service", id, price, width, height, matrix),
			YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "no service", id, price, width, height, matrix),
			YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id, price, width, height, matrix),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id+1, price, width, height, matrix),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id+1, price, width, height, matrix),
			YAD3_APARTMENT_ALREADY_EXISTS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id+2, price, width, height, matrix),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,
			"realtor1@gmail.com", "housing", id+3, price, width, height, matrix),
			YAD3_APARTMENT_SERVICE_FULL);

	return final;
}

static bool yad3serviceRealtorRemoveApartmentTest(yad3service yad3, int id){
	bool final = true;

	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"bad email.com", "housing", id), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"realtor1@gmail.com", "housing", -1),	YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"noRealtor@gmail.com", "housing", id),	YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"customer1@gmail.com", "housing", id),
			YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"realtor1@gmail.com", "no service", id),
			YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"realtor1@gmail.com", "housing", id-1),
			YAD3_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"realtor1@gmail.com", "housing", id+1), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartment(yad3,
			"realtor1@gmail.com", "housing", id+1),
			YAD3_APARTMENT_DOES_NOT_EXIST);

	return final;
}

static bool yad3serviceCustomerPurchaseTest(yad3service yad3, int id,
		int max_price, int width, int height, char* matrix, int expensive_id){
	bool final = true;

	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"bad email.com", "realtor1@gmail.com", "housing",
			id), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "bad email.com", "housing",
			id), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			-1), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"noCustomer@gmail.com", "realtor1@gmail.com", "housing",
			id), YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "noRealtor@gmail.com", "housing",
			id), YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "customer1@gmail.com", "housing",
			id), YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"realtor1@gmail.com", "realtor1@gmail.com", "housing",
			id), YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,	// <<<<
			"customer1@gmail.com", "realtor1@gmail.com", "no service",
			id), YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			id-3), YAD3_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			id-3), YAD3_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,	// adding apartment
			"realtor1@gmail.com", "housing", expensive_id, max_price+1, width,
			height, matrix), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			expensive_id), YAD3_PURCHASE_WRONG_PROPERTIES);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			id+2), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceCustomerPurchase(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			id+2), YAD3_APARTMENT_DOES_NOT_EXIST);

	return final;
}


static bool yad3serviceCustomerMakeOfferTest(yad3service yad3, int safe_id,
		int id, int price, int width, int height, char* matrix, int max_price){
	bool final = true;

	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"bad email.com", "realtor1@gmail.com", "housing",
			id, price-1), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer1@gmail.com", "bad mail.com", "housing",
			id, price-1), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			-id, price-1), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			id, 0), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"noCustomer@gmail.com", "realtor1@gmail.com", "housing",
			id, price-1), YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer1@gmail.com", "noRealtor@gmail.com", "housing",
			id, price-1), YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer1@gmail.com", "customer1@gmail.com", "housing",
			id, price-1), YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"realtor1@gmail.com", "customer1@gmail.com", "housing",
			id, price-1), YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRealtorAddApartment(yad3,	// adding apartment
			"realtor1@gmail.com", "housing", safe_id, price, width,
			height, matrix), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			safe_id, price-1), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer1@gmail.com", "realtor1@gmail.com", "housing",
			safe_id, price-1), YAD3_ALREADY_REQUESTED);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer2@gmail.com", "realtor1@gmail.com", "housing",
			safe_id+30, price-1), YAD3_APARTMENT_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer2@gmail.com", "realtor1@gmail.com", "no service",
			safe_id, price-1), YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer2@gmail.com", "realtor1@gmail.com", "housing",
			safe_id, max_price+1), YAD3_REQUEST_WRONG_PROPERTIES);
	TEST_EQUALS(final, yad3serviceCustomerMakeOffer(yad3,
			"customer2@gmail.com", "realtor1@gmail.com", "housing",
			safe_id, price+2), YAD3_REQUEST_ILLOGICAL_PRICE);

	return final;
}

static bool yad3serviceRealtorRespondToOfferTest(yad3service yad3){
	bool final = true;

	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"bad email.com", "customer1@gmail.com", accept),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor1@gmail.com", "customer1@gmail.com", unknown),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor11@gmail.com", "bad mail.com", accept),
			YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor1@gmail.com", "noCustomer@gmail.com", accept),
			YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"noRealtor@gmail.com", "customer1@gmail.com", accept),
			YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor1@gmail.com", "realtor1@gmail.com", accept),
			YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"customer1@gmail.com", "customer1@gmail.com", accept),
			YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor2@gmail.com", "customer1@gmail.com", accept),
			YAD3_NOT_REQUESTED);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor1@gmail.com", "customer2@gmail.com", accept),
			YAD3_NOT_REQUESTED);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor1@gmail.com", "customer1@gmail.com", accept),
			YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceRealtorRespondToOffer(yad3,
			"realtor1@gmail.com", "customer1@gmail.com", accept),
			YAD3_NOT_REQUESTED);

	return final;
}

static bool yad3serviceReportRelevantRealtorsTest(yad3service yad3,
		FILE* output, char* matrix, int width, int height){
	bool final = true;

	TEST_EQUALS(final, yad3serviceReportRelevantRealtors(yad3,
			"bad mail.com", output), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceReportRelevantRealtors(yad3,
			"noCustomer@gmail.com", output), YAD3_EMAIL_DOES_NOT_EXIST);
	TEST_EQUALS(final, yad3serviceReportRelevantRealtors(yad3,
			"customer1@gmail.com", output), YAD3_SUCCESS);
	int low_price = 5, new_id = 500;
	yad3serviceRealtorAddApartment(yad3, "realtor2@gmail.com", "housing",
			new_id, low_price, width, height, matrix);	// second realtor
	TEST_EQUALS(final, yad3serviceReportRelevantRealtors(yad3,
			"customer1@gmail.com", output), YAD3_SUCCESS);
	return final;
}

static bool yad3serviceReportMostPayingCustomersTest(yad3service yad3,
		FILE* output){
	bool final = true;

	TEST_EQUALS(final, yad3serviceReportMostPayingCustomers(yad3,
			0, output), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceReportMostPayingCustomers(yad3,
			1, output), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceReportMostPayingCustomers(yad3,
			1, output), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceReportMostPayingCustomers(yad3,
			3, output), YAD3_SUCCESS);

	return final;
}

static bool yad3serviceReportSignificantRealtorsTest(yad3service yad3,
		FILE* output){
	bool final = true;

	TEST_EQUALS(final, yad3serviceReportSignificantRealtors(yad3,
			0, output), YAD3_INVALID_PARAMETERS);
	TEST_EQUALS(final, yad3serviceReportSignificantRealtors(yad3,
			5, output), YAD3_SUCCESS);
	TEST_EQUALS(final, yad3serviceReportSignificantRealtors(yad3,
			3, output), YAD3_SUCCESS);

	return final;
}




bool yad3Test(){
	bool final = true;

	// -------------<yad3serviceCreate>-------------
	yad3service yad3 = yad3serviceCreate();
	// -------------</yad3serviceCreate>-------------



	// -------------<yad3serviceAddRealtor>-------------
	TEST_EQUALS(final, yad3serviceAddRealtorTest(yad3), true);
	TEST_RESULT(yad3serviceAddRealtorTest, final);
	// -------------</yad3serviceAddRealtor>-------------



	// -------------<yad3serviceAddCustomer>-------------
	int min_area = 1, min_rooms = 1, max_price = 10000000;
	TEST_EQUALS(final, yad3serviceAddCustomerTest(yad3, min_area, min_rooms,
			max_price), true);
	TEST_RESULT(yad3serviceAddCustomerTest, final);
	// -------------</yad3serviceAddCustomer>-------------



	// -------------<yad3serviceRemoveRealtor>-------------
	TEST_EQUALS(final, yad3serviceRemoveRealtorTest(yad3), true);
	TEST_RESULT(yad3serviceRemoveRealtorTest, final);
	// -------------</yad3serviceRemoveRealtor>-------------



	// -------------<yad3serviceRemoveCustomer>-------------
	TEST_EQUALS(final, yad3serviceRemoveCustomerTest(yad3), true);
	TEST_RESULT(yad3serviceRemoveCustomerTest, final);
	// -------------</yad3serviceRemoveCustomer>-------------



	// -------------<yad3serviceRealtorAddApartmentService>-------------
	int max_apartments = 3;
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentServiceTest(yad3,
			max_apartments), true);
	TEST_RESULT(yad3serviceRealtorAddApartmentServiceTest, final);
	// -------------</yad3serviceRealtorAddApartmentService>-------------




	// -------------<yad3serviceRealterRemoveApartmentService>-------------
	TEST_EQUALS(final, yad3serviceRealterRemoveApartmentServiceTest
			(yad3), true);
	TEST_RESULT(yad3serviceRealterRemoveApartmentServiceTest, final);

	// -------------</yad3serviceRealterRemoveApartmentService>-------------



	// -------------<yad3serviceRealtorAddApartment>-------------
	int id = 10, price = 1315500, height = 4, width = 4;
	char* matrix = "weewwwewwewwweee";
	TEST_EQUALS(final, yad3serviceRealtorAddApartmentTest(yad3, id, price,
			width, height, matrix), true);	// <<<<<<<<<<<<<<<<<<<<<<<<<<
	TEST_RESULT(yad3serviceRealtorAddApartment, final);
	// -------------</yad3serviceRealtorAddApartment>-------------



	// -------------<yad3serviceRealtorRemoveApartment>-------------
	TEST_EQUALS(final, yad3serviceRealtorRemoveApartmentTest
			(yad3, id), true);
	TEST_RESULT(yad3serviceRealtorRemoveApartmentTest, final);
	// -------------</yad3serviceRealtorRemoveApartment>-------------



	// -------------<yad3serviceCustomerPurchase>-------------
	int expensive_id = id+1;
	TEST_EQUALS(final, yad3serviceCustomerPurchaseTest
			(yad3, id, max_price, width, height, matrix, expensive_id), true);
	TEST_RESULT(yad3serviceCustomerPurchaseTest, final);
	// -------------</yad3serviceCustomerPurchase>-------------



	// -------------<yad3serviceCustomerMakeOffer>-------------
	int safe_id = 100;	// safe from deletion
	TEST_EQUALS(final, yad3serviceCustomerMakeOfferTest
			(yad3, safe_id, id, price, width, height, matrix, max_price),
			true);
	TEST_RESULT(yad3serviceCustomerMakeOfferTest, final);
	// -------------</yad3serviceCustomerMakeOffer>-------------



	// -------------<yad3serviceRealtorRespondToOffer>-------------
	TEST_EQUALS(final, yad3serviceRealtorRespondToOfferTest(yad3), true);
	TEST_RESULT(yad3serviceRealtorRespondToOfferTest, final);
	// -------------</yad3serviceRealtorRespondToOffer>-------------



	// -------------<yad3serviceReportRelevantRealtors>-------------
	TEST_EQUALS(final, yad3serviceReportRelevantRealtorsTest(yad3, stdout,
			matrix, width, height), true);
	TEST_RESULT(yad3serviceReportRelevantRealtorsTest, final);
	// -------------</yad3serviceRealtorRespondToOffer>-------------



	// -------------<yad3serviceReportMostPayingCustomersTest>-------------
	TEST_EQUALS(final, yad3serviceReportMostPayingCustomersTest(yad3, stdout)
			, true);
	TEST_RESULT(yad3serviceReportMostPayingCustomersTest, final);
	// -------------</yad3serviceReportMostPayingCustomersTest>-------------




	// -------------<yad3serviceReportSignificantRealtors>-------------
	TEST_EQUALS(final, yad3serviceReportSignificantRealtorsTest(yad3, stdout),
			true);
	TEST_RESULT(yad3serviceReportSignificantRealtorsTest, final);
	// -------------</yad3serviceReportSignificantRealtors>-------------



	// -------------<yad3serviceDestroy>-------------
	yad3serviceDestroy(yad3);
	// -------------</yad3serviceDestroy>-------------


	return final;
}

int main1(){
	//setvbuf(stdout, NULL, _IONBF, 0);
	//setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(yad3Test);
	return 0;
}
