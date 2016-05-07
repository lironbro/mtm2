/*
 * User.h
 *
 *  Created on: 30 באפר׳ 2016
 *      Author: Liron
 */


#ifndef USER_H_
#define USER_H_

#include "Realtor.h"
#include "Customer.h"

typedef void* userElement;


typedef enum userType_t{
	realtor, customer,
} userType;


typedef struct user_t* User;
typedef enum userResult_t {
	USER_INVALID_PARAMETERS,	// Realtor errors
	USER_EMAIL_ALREADY_EXISTS,
	USER_EMAIL_DOES_NOT_EXIST,
	USER_EMAIL_WRONG_ACCOUNT_TYPE,
	USER_APARTMENT_SERVICE_ALREADY_EXISTS,
	USER_APARTMENT_SERVICE_DOES_NOT_EXIST,
	USER_APARTMENT_SERVICE_FULL,
	USER_APARTMENT_ALREADY_EXISTS,
	USER_APARTMENT_DOES_NOT_EXIST,
	USER_PURCHASE_WRONG_PROPERTIES,	// Customer errors
	USER_ALREADY_REQUESTED,
	USER_REQUEST_WRONG_PROPERTIES,
	USER_REQUEST_ILLOGICAL_PRICE,
	USER_NOT_REQUESTED,
	USER_SUCCESS,
	USER_OUT_OF_MEMORY,
	USER_NULL_ARGUMENT,
	USER_REQUEST_WRONG_PPROPERTIES
} userResult;


// Realtor

User userCopy(User user);

void userDestroy(User user);

User userRealtorCreate(char* company_name, int tax_percentage);

userResult userRealtorDestroy(User user);

userResult userRealtorAddService(User user, char* service_name, int max_apartments);

userResult userRealtorRemoveService(User user, char* service_name);

userResult userRealtorAddApartment(User user, char* service_name,
		int id, int price, int width, int height, char* matrix);

userResult userRealtorRemoveApartment(User user, char* service_name, int id);

userResult userRealtorRespondToOffer(User userRealtor,
		User userCustomer, char* customer_email, Choice choice);

bool userIsRealtorRelevant(User user, Customer target);


// Customer
User userCustomerCreate(int min_area, int min_rooms, int max_price);

userResult userCustomerDestroy(User user);

userResult userCustomerPurchase(User userCustomer, User userRealtor,
		char* service_name, int apartment_id);

userResult userCustomerMakeOffer(User userCustomer, User userRealtor, int id, int price,
		char* service_name, char* customer_email);

userType userGetType(User user);

userElement userGetData(User user);

#endif /* USER_H_ */
