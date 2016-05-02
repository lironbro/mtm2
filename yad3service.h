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
#include "apartment_service.h"
#include "User.h"
#include "Offer.h"
#include "mtm_ex2.h"		// <<<<<<<< these are the types of errors we should return


/** Type for defining the map */
typedef struct yad3service_t *yad3service;


/** Type used for returning error codes from yad3 functions */
typedef enum yad3Result_t {
	YAD3_INVALID_PARAMETERS,	// Realtor errors
	YAD3_EMAIL_ALREADY_EXISTS,
	YAD3_EMAIL_DOES_NOT_EXIST,
	YAD3_EMAIL_WRONG_ACCOUNT_TYPE,
	YAD3_APARTMENT_SERVICE_ALREADY_EXISTS,
	YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST,
	YAD3_APARTMENT_SERVICE_FULL,
	YAD3_APARTMENT_ALREADY_EXISTS,
	YAD3_APARTMENT_DOES_NOT_EXIST,
	YAD3_PURCHASE_WRONG_PROPERTIES,	// Customer errors
	YAD3_ALREADY_REQUESTED,
	YAD3_REQUEST_WRONG_PROPERTIES,
	YAD3_REQUEST_ILLOGICAL_PRICE,
	YAD3_NOT_REQUESTED,
	YAD3_SUCCESS,
	YAD3_OUT_OF_MEMORY,
	YAD3_NULL_ARGUMENT,
	YAD3_REQUEST_WRONG_PPROPERTIES
} yad3Result;

typedef enum choice_t{	// should be accessible to both customer and realtor
	accept, decline,
} Choice;





yad3service yad3serviceCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement,
		freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement,
		compareMapKeyElements compareKeyElements);	// define parameters later ---------------------------

void yad3serviceDestroy(yad3service yad3);

yad3Result yad3serviceAddRealtor(yad3service yad3,
		char* email, char* company_name, int tax_percentage);

yad3Result yad3serviceRemoveRealtor(yad3service yad3, char* email);

yad3Result yad3serviceRealtorAddApartmentService
		(yad3service yad3, char* email, char* service_name, int max_apartments);

yad3Result yad3serviceRealterRemoveApartmentService
		(yad3service yad3, char* email, char* service_name);

yad3Result yad3serviceRealtorAddApartment
		(yad3service yad3, char* email, char* service_name,
				int id, int price, int width, int height, char* matrix);

yad3Result yad3serviceRealtorRemoveApartment
		(yad3service yad3, char* email, char* service_name, int id);

yad3Result yad3serviceAddCustomer
		(yad3service yad3, char* email, int min_area, int min_rooms, int max_price);

yad3Result yad3serviceRemoveCustomer(yad3service yad3, char* email);

yad3Result yad3serviceCustomerPurchase
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id);

yad3Result yad3serviceCustomerMakeOffer
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id, int new_price);

yad3Result yad3serviceRealtorRespondToOffer
		(yad3service yad3, char* email, char* customer_email, Choice choice);

yad3Result yad3serviceReportRelevantRealtors(yad3service yad3, char* customer_email);

yad3Result yad3serviceReportMostPayingCustomers(yad3service yad3, int count);

yad3Result yad3serviceReportSignificantRealtors(yad3service yad3, int count);

#endif /* YAD3SERVICE_H_ */
