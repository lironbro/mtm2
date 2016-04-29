/*
 * yad3service.h
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#ifndef YAD3SERVICE_H_
#define YAD3SERVICE_H_

#include <map.h>
#include <list.h>
#include <set.h>
#include <stdio.h>
#include <stdlib.h>
#include "apartment_service.h"

/** Type for defining the map */
typedef struct yad3service_t *yad3service;


/** Type used for returning error codes from yad3 functions */
typedef enum MTMResult_t {
	MTM_INVALID_PARAMETERS,	// Realtor errors
	MTM_EMAIL_ALREADY_EXISTS,
	MTM_EMAIL_DOES_NOT_EXIST,
	MTM_EMAIL_WRONG_ACCOUNT_TYPE,
	MTM_APARTMENT_SERVICE_ALREADY_EXISTS,
	MTM_APARTMENT_SERVICE_DOES_NOT_EXIST,
	MTM_APARTMENT_SERVICE_FULL,
	MTM_APARTMENT_ALREADY_EXISTS,
	MTM_APARTMENT_DOES_NOT_EXIST,
	MTM_PURCHASE_WRONG_PROPERTIES,	// Customer errors
	MTM_ALREADY_REQUESTED,
	MTM_REQUEST_WRONG_PROPERTIES,
	MTM_REQUEST_ILLOGICAL_PRICE,
	MTM_NOT_REQUESTED,
	MTM_SUCCESS,
	MTM_OUT_OF_MEMORY
} MTMResult;

typedef enum choice_t{	// should be accessible to both customer and realtor
	accept, decline,
} Choice;

// I think this should work
typedef struct Customer_t* Customer;
typedef struct Realtor_t* Realtor;


yad3service yad3serviceCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement,
		freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement,
		compareMapKeyElements compareKeyElements);	// define parameters later ---------------------------

void yad3serviceDestroy();

MTMResult yad3serviceAddRealtor(char* email, char* company_name, int tax_percentage);

MTMResult yad3serviceRemoveRealtor(char* email);

MTMResult yad3serviceRealtorAddApartmentService
		(char* email, char* service_name, int max_apartments);

MTMResult yad3serviceRealterRemoveApartmentService
		(char* email, char* service_name);

MTMResult yad3serviceRealtorAddApartment
		(char* email, char* service_name, int id, int price, int width,
				int height, char* matrix);

MTMResult yad3serviceRealtorRemoveApartment
		(char* email, char* service_name, int id);

MTMResult yad3serviceAddCustomer
		(char* email, int min_area, int min_rooms, int max_price);

MTMResult yad3serviceRemoveCustomer(char* email);

MTMResult yad3serviceCustomerPurchase
		(char* email, char* realtor_email,
				char* service_name, int apartment_id);

MTMResult yad3serviceCustomerMakeOffer
		(char* email, char* realtor_email,
				char* service_name, int apartment_id, int new_price);

MTMResult yad3serviceRealtorRespondToOffer
		(char* email, char* customer_email, Choice choise);

MTMResult yad3serviceReportRelevantRealtors(char* customer_email);

MTMResult yad3serviceReportMostPayingCustomers(int count);

MTMResult yad3serviceReportSignificantRealtors(int count);

#endif /* YAD3SERVICE_H_ */
