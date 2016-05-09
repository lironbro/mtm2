/*
 * User.h
 *
 *  Created on: 30 באפר׳ 2016
 *      Author: Liron
 */


/*
 * final header file
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

/*
 * userCopy: copies a user
 *
 * @param user: the user in need of copying
 */
User userCopy(User user);


/*
 * userDestroy: destroys a user
 *
 * @param user: the user in need of destroyings
 */
void userDestroy(User user);

/*
 * userRealtorCreate: creates a realtor with the given parameters
 *
 * @param company_name: the comppany name
 * @param tax_percentage: the tax percentage
 */
User userRealtorCreate(char* company_name, int tax_percentage);

/*
 * userRealtorCopy: creates a realtor type-user out of a copy of
 * the given realtor
 */
User userRealtorCopy(Realtor originalRealtor);

/*
 * userRealtorDestroy: destroys a realtor
 *
 * @param user: the user (realtor) in need of destroying
 *
 * returns:
 * USER_EMAIL_WRONG_ACCOUNT_TYPE if the user isn't a customer
 * USER_SUCCESS if the destroying went as planned
 */
userResult userRealtorDestroy(User user);

/*
 * userRealtorAddService: adds a service to a given user
 *
 * @param user: the user to whom we will want to add the service
 * @param service_name: the name of the new service
 * @param max_apartments: the maximum apartments to be held in the service
 *
 * returns:
 * USER_SUCCESS if the service has been added successfully
 * USER_EMAIL_WRONG_ACCOUNT_TYPE if the user isn't a realtor
 *
 * USER_INVALID_PARAMETERS if wrong parameters have been sent
 * USER_APARTMENT_SERVICE_ALREADY_EXISTS if there already exists a service
 * 				with such a name
 * USER_OUT_OF_MEMORY if out of memory
 */
userResult userRealtorAddService(User user, char* service_name,
		int max_apartments);

/*
 * userRealtorRemoveService: adds a service to a given user
 *
 * @param user: the user from whom we will want to remove the service
 * @param service_name: the name of the service
 *
 * returns:
 * USER_SUCCESS if the service has been removed successfully
 * USER_EMAIL_WRONG_ACCOUNT_TYPE if the user isn't a realtor
 * USER_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no service with such a name
 */
userResult userRealtorRemoveService(User user, char* service_name);

/*
 * userRealtorAddApartment: adds an apartment to a service of a given user
 *
 * @param user: the user to whom we will want to add the apartment
 * @param service_name: the name of the service
 * @param id: the id of the new apartmment
 * @param price: the price of the new apartment
 * @param width: the width of the new apartment
 * @param height: the height of the new apartment
 * @param matrix: the matrix symbolizing the square structre of the apartment
 *
 * returns:
 * USER_SUCCESS if the apartment has been added successfully
 * USER_EMAIL_WRONG_ACCOUNT_TYPE if the user isn't a realtor
 * USER_INVALID_PARAMETERS if wrong parameters have been sent
 * USER_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no service with such a name
 * USER_APARTMENT_ALREADY_EXISTS if an apartment with such an id already exists
 * USER_APARTMENT_SERVICE_FULL if the service is full
 * USER_OUT_OF_MEMORY if out of memory
 */
userResult userRealtorAddApartment(User user, char* service_name,
		int id, int price, int width, int height, char* matrix);

/*
 * userRealtorRemoveApartment: removes an apartment to a service of a given user
 *
 * @param user: the user to whom we will want to add the apartment
 * @param service_name: the name of the service
 * @param id: the id of the apartmment
 *
 * returns:
 * USER_SUCCESS if the apartment has been removed successfully
 * USER_EMAIL_WRONG_ACCOUNT_TYPE if the user isn't a realtor
 * USER_INVALID_PARAMETERS if wrong parameters have been sent
 * USER_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no service with such a name
 * USER_APARTMENT_DOES_NOT_EXIST if there is no apartment with such an id
 */
userResult userRealtorRemoveApartment(User user, char* service_name, int id);

/*
 * userRealtorRespondToOffer: makes the realtor respond to an offer given by
 * 			the user with the specified email. a legal Choice (i.e. the
 * 			realtor's response, can be accept or decline)
 *
 * 	@param userRealtor: the realtor who sends the response
 * 	@param userCustomer: the customer who sent the offer
 * 	@param customer_email: the email of the customer
 * 	@param choice: accept or decline, which are legal, or unknown which is isn't
 *
 * 	returns:
 * 	USER_SUCCESS if the response has been made successfully
 * 	USER_EMAIL_WRONG_ACCOUNT_TYPE if a user is the wrong type
  * USER_INVALID_PARAMETERS if wrong parameters have been sent
 *  USER_NOT_REQUESTED if no request has been made by the user to the realtor
 *
 *
 */
userResult userRealtorRespondToOffer(User userRealtor,
		User userCustomer, char* customer_email, Choice choice);

/*
 * userIsRealtorRelevant: returns whether a realtor is relevant to the target
 * 			customer
 *
 * @param user: the user which which may or may not be relevant to target,
 * 			and may or may not be a realtor at all
 * @param target: the target customer
 *
 * returns:
 * true if user is a realtor and is relevant to target
 * false otherwise
 */
bool userIsRealtorRelevant(User user, Customer target);


// Customer

/*
 * userCustomerCreate: creates a customer
 */
User userCustomerCreate(int min_area, int min_rooms, int max_price);

/*
 * userCustomerCopy: creates a customer-type copy user out of a given customer
 */
User userCustomerCopy(Customer customer);

/*
 * userCustomerDestroy: destroys a customer
 */
userResult userCustomerDestroy(User user);

/*
 * userCustomerPurchase: purchases an apartment for userCustomer from
 * 			userRealtor in service_name with the given id
 *
 * @param userCustomer: the customer
 * @param userRealtor: the realtor
 * @param service_name: the service in which the apartment is being bought
 * @param apartment_id: the id of the apartment being bought
 *
 * returns:
 * USER_SUCCESS if the apartment was purchased successfully
 * USER_EMAIL_WRONG_ACCOUNT_TYPE if the user isn't a realtor
 * USER_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no service with such a name
 * USER_APARTMENT_DOES_NOT_EXIST if there is no apartment with such an id
 * USER_PURCHASE_WRONG_PROPERTIES if the apartment's properties don't fit
 *
 */
userResult userCustomerPurchase(User userCustomer, User userRealtor,
		char* service_name, int apartment_id);

/*
 * userCustomerMakeOffer: sends an offer to a realtor for the given apartment
 *
 * @param userCustomer: the customer
 * @param userRealtor: the realtor
 * @param service_name: the service in which the apartment is being bought
 * @param id: the id of the apartment being bought
 * @param price: the price being offered by the customer for the apartment
 *
 * returns:
 * USER_SUCCESS if the apartment was purchased successfully
 * USER_EMAIL_WRONG_ACCOUNT_TYPE if the user isn't a realtor
 * USER_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no service with such a name
 * USER_APARTMENT_DOES_NOT_EXIST if there is no apartment with such an id
 * USER_REQUEST_WRONG_PROPERTIES if the apartment's properties don't fit
 * USER_ALREADY_REQUESTED if a request has already been sent by this user
 * USER_REQUEST_ILLOGICAL_PRICE if the price doesn't means sense
 */
userResult userCustomerMakeOffer(User userCustomer, User userRealtor,
		char* customer_email, char* service_name, int id, int price);

/*
 * userGetType: returns the user's type, realtor or customer
 */
userType userGetType(User user);

/*
 * userGetData: returns a pointer to the user's data
 */
userElement userGetData(User user);

#endif /* USER_H_ */
