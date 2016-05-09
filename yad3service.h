/*
 * yad3service.h
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#ifndef YAD3SERVICE_H_
#define YAD3SERVICE_H_

#include "map.h"
#include "list.h"
#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include "mtm_ex2.h"
#include "User.h"

/** Type for defining the map */
typedef struct yad3service_t *yad3service;


/** Type used for returning error codes from yad3 functions */
typedef enum yad3Result_t {
	YAD3_INVALID_PARAMETERS,
	YAD3_EMAIL_ALREADY_EXISTS,
	YAD3_EMAIL_DOES_NOT_EXIST,
	YAD3_EMAIL_WRONG_ACCOUNT_TYPE,
	YAD3_APARTMENT_SERVICE_ALREADY_EXISTS,
	YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST,
	YAD3_APARTMENT_SERVICE_FULL,
	YAD3_APARTMENT_ALREADY_EXISTS,
	YAD3_APARTMENT_DOES_NOT_EXIST,
	YAD3_PURCHASE_WRONG_PROPERTIES,
	YAD3_ALREADY_REQUESTED,
	YAD3_REQUEST_WRONG_PROPERTIES,
	YAD3_REQUEST_ILLOGICAL_PRICE,
	YAD3_NOT_REQUESTED,
	YAD3_SUCCESS,
	YAD3_OUT_OF_MEMORY,
	YAD3_NULL_ARGUMENT,
	YAD3_REQUEST_WRONG_PPROPERTIES
} yad3Result;


typedef int(*compareFunction)(char*, char*);

/*
 * yad3serviceCreate: creates a yad3service
 */
yad3service yad3serviceCreate();
void yad3serviceDestroy(yad3service yad3);

/*
 * yad3serviceAddRealtor: adds a realtor to yad3
 *
 * @param yad3: the yad3service
 * @param email: the email address of the new realtor
 * @param company_name: the name of the company
 * @param tax_percentage: the tax percentage
 *
 * returns:
 * YAD3_SUCCESS if the realtor has been added successfully
 * YAD3_EMAIL_ALREADY_EXISTS if a user with the given email address already
 * 			exists in yad3
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 */
yad3Result yad3serviceAddRealtor(yad3service yad3,
		char* email, char* company_name, int tax_percentage);

/*
 * yad3serviceRemoveRealtor: removes a realtor from yad3
 *
 * @param yad3: the yad3service
 * @param email: the email address of the realtor
 *
 * returns:
 * YAD3_SUCCESS if the realtor has been removed successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 */
yad3Result yad3serviceRemoveRealtor(yad3service yad3, char* email);

/*
 * yad3serviceRealtorAddApartmentService: adds an apartment service to realtor
 *
 * @param yad3: the yad3service
 * @param email: the email address of the new realtor
 * @param service_name: the name of the service
 * @param max_apartments: the maximum number of apartments in the new service
 *
 * returns:
 * YAD3_SUCCESS if the apartment service has been added successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 * YAD3_APARTMENT_SERVICE_ALREADY_EXISTS if a service with the given name
 * 			already exists
 */
yad3Result yad3serviceRealtorAddApartmentService
		(yad3service yad3, char* email, char* service_name, int max_apartments);

/*
 * yad3serviceRealterRemoveApartmentService: removes an apartment service
 *
 * @param yad3: the yad3service
 * @param email: the email address of the realtor
 * @param service_name: the name of the service
 *
 * returns:
 * YAD3_SUCCESS if the apartment service has been added successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 * YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST if a service with the given name
 * 			does not exist
 */
yad3Result yad3serviceRealterRemoveApartmentService
		(yad3service yad3, char* email, char* service_name);

/*
 * yad3serviceRealtorAddApartment: adds an apartment to realtor
 *
 * @param yad3: the yad3service
 * @param email: the email address of the realtor
 * @param service_name: the name of the service
 * @param id: the id of the new apartment
 * @param price: the price of the new apartment
 * @param width: the width
 * @param height: the height
 * @param matrix: the matrix representing the square structre of
 * 			the new apartment
 *
 * returns:
 * YAD3_SUCCESS if the apartment has been added successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 * YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST if a service with the given name
 * 			does not exist
 * YAD3_APARTMENT_SERVICE_FULL if the service is full
 * YAD3_APARTMENT_ALREADY_EXISTS if there is already an apartment with the
 * 			given id in the given service
 */
yad3Result yad3serviceRealtorAddApartment
		(yad3service yad3, char* email, char* service_name,
				int id, int price, int width, int height, char* matrix);

/*
 * yad3serviceRealtorRemoveApartment: removes an apartment from realtor
 *
 * @param yad3: the yad3service
 * @param email: the email address of the realtor
 * @param service_name: the name of the service
 * @param id: the id of the new apartment
 *
 * returns:
 * YAD3_SUCCESS if the apartment service has been added successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 * YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST if a service with the given name
 * 			does not exist
 * YAD3_APARTMENT_DOES_NOT_EXIST if there is no apartment with the
 * 			given id in the given service
 */
yad3Result yad3serviceRealtorRemoveApartment
		(yad3service yad3, char* email, char* service_name, int id);

/*
 * yad3serviceAddCustomer: adds a customer to yad3
 *
 * @param yad3: the yad3service
 * @param email: the email address of the new customer
 * @param min_area: the minimum area for the customer
 * @param min_rooms: the minimum rooms for the customer
 * @param max_price: the maximum price for the customer
 *
 * returns:
 * YAD3_SUCCESS if the customer has been added successfully
 * YAD3_EMAIL_ALREADY_EXISTS if a user with the given email address already
 * 			exists in yad3
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 */
yad3Result yad3serviceAddCustomer
		(yad3service yad3, char* email, int min_area, int min_rooms,
				int max_price);

/*
 * yad3serviceRemoveRealtor: removes a customer from yad3
 *
 * @param yad3: the yad3service
 * @param email: the email address of the customer
 *
 * returns:
 * YAD3_SUCCESS if the customer has been removed successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 */
yad3Result yad3serviceRemoveCustomer(yad3service yad3, char* email);

/*
 * yad3serviceCustomerPurchase: purchases an apartment for the customer
 *
 * @param yad3: the yad3service
 * @param email: the email address of the customer
 * @param realtor_email: the email of the realtor
 * @param service_name: the name of the apartment service
 * @param apartment_id: the id of the apartment
 *
 * returns:
 * YAD3_SUCCESS if the apartment has been purchased successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 * YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST if a service with the given name
 * 			does not exist
 * YAD3_APARTMENT_DOES_NOT_EXIST if there is no apartment with the
 * 			given id in the given service
 * YAD3_PURCHASE_WRONG_PROPERTIES if the properties of the apartment are wrong
 */
yad3Result yad3serviceCustomerPurchase
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id);

/*
 * yad3serviceCustomerMakeOffer: purchases an apartment for the customer
 *
 * @param yad3: the yad3service
 * @param email: the email address of the customer
 * @param realtor_email: the email of the realtor
 * @param service_name: the name of the apartment service
 * @param apartment_id: the id of the apartment
 * @param new_price: the price being offered for the apartment
 *
 * returns:
 * YAD3_SUCCESS if the offer has been made successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 * YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST if a service with the given name
 * 			does not exist
 * YAD3_APARTMENT_DOES_NOT_EXIST if there is no apartment with the
 * 			given id in the given service
 * YAD3_REQUEST_WRONG_PPROPERTIES if the properties of the apartment are wrong
 * YAD3_ALREADY_REQUESTED if a request has already been sent by this customer
 * YAD3_REQUEST_ILLOGICAL_PRICE if the offer's price doesn't make sense
 */
yad3Result yad3serviceCustomerMakeOffer
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id, int new_price);

/*
 * yad3serviceRealtorRespondToOffer: purchases an apartment for the customer
 *
 * @param yad3: the yad3service
 * @param email: the email address of the realtor
 * @param customer_email: the email of the customer
 * @param service_name: the name of the apartment service
 * @param choice: the response of the realtor. a legal response will be
 * 			accept or decline, otherwise it'll be unknown
 *
 * returns:
 * YAD3_SUCCESS if the offer has been made successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 * YAD3_REQUEST_WRONG_PPROPERTIES if the properties of the apartment are wrong
 */
yad3Result yad3serviceRealtorRespondToOffer
		(yad3service yad3, char* email, char* customer_email, Choice choice);

/*
 * yad3serviceReportRelevantRealtors: prints the relevant realtors to a given
 * 			custoer
 *
 * @param yad3: the yad3service
 * @param customer_email: the email address of the customer for whom we want
 * 			to print the relevant realtors
 * @param output: the file to which the realtors will be printed out
 *
 * returns:
 * YAD3_SUCCESS if the offer has been made successfully
 * YAD3_EMAIL_DOES_NOT_EXIST if a user with the given email address
 * 			does not exist
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 * YAD3_WRONG_ACCOUNT_TYPE if the user with the given email address is
 * 			of a different type than expected
 */
yad3Result yad3serviceReportRelevantRealtors(yad3service yad3,
		char* customer_email, FILE* output);

/*
 * yad3serviceReportMostPayingCustomers: reports up to count most paying
 * 			customers
 *
 * @param yad3: the yad3service
 * @param count: the number of customers which need to be printed
 * @param output: the file to which the customers will be printed
 *
 * returns
 * YAD3_SUCCESS if the offer has been made successfully
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 */
yad3Result yad3serviceReportMostPayingCustomers(yad3service yad3, int count,
		FILE* output);

/*
 * yad3serviceReportSignificantRealtors: reports up to count most significant
 * 			realtors
 *
 * @param yad3: the yad3service
 * @param count: the number of realtors which need to be printed
 * @param output: the file to which the realtors will be printed
 *
 * returns
 * YAD3_SUCCESS if the offer has been made successfully
 * YAD3_INVALID_PARAMETERS: if wrong parameters have been sent
 */
yad3Result yad3serviceReportSignificantRealtors(yad3service yad3, int count,
		FILE* output);

/*
 * yad3serviceGetEmails: returns the list of email in yad3
 */
Map yad3serviceGetEmails(yad3service yad3);

#endif /* YAD3SERVICE_H_ */
