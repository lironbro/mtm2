/*
 * Realtor.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#ifndef REALTOR_H_
#define REALTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Offer.h"
#include "map.h"
//#include "list.h"
#include "set.h"
#include "apartment_service.h"
#include "apartment.h"
#include <string.h>


typedef struct Realtor_t* Realtor;


typedef enum choice_t{	// should be accessible to both yad3service and realtor
	accept, decline,
} Choice;

typedef enum realtorResult_t{
	REALTOR_INVALID_PARAMETERS,	// Realtor errors
	REALTOR_EMAIL_ALREADY_EXISTS,
	REALTOR_EMAIL_DOES_NOT_EXIST,
	REALTOR_EMAIL_WRONG_ACCOUNT_TYPE,
	REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS,
	REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST,
	REALTOR_APARTMENT_SERVICE_FULL,
	REALTOR_APARTMENT_ALREADY_EXISTS,
	REALTOR_APARTMENT_DOES_NOT_EXIST,
	REALTOR_SUCCESS,
	REALTOR_OUT_OF_MEMORY,
	REALTOR_NULL_ARGUMENT,
	REALTOR_NOT_REQUESTED
} realtorResult;

Realtor realtorCreate(char* company_name, int tax_percentage);

Realtor realtorCopy(Realtor realtor);

void realtorDestroy(Realtor realtor);

realtorResult realtorAddService
		(Realtor realtor, char* service_name, int max_apartments);

realtorResult realtorRemoveService(Realtor realtor, char* service_name);

realtorResult realtorAddApartment(Realtor realtor, char* service_name,
		int id, int price, int width, int height, char* matrix);

realtorResult realtorRemoveApartment(Realtor realtor, char* service_name, int id);

realtorResult realtorRespondToOffer
		(Realtor realtor, char* customer_email, Choice choice);

bool realtorIsRealtorRelevant(Realtor realtor, int customer_min_area, int customer_min_rooms, int customer_max_price);

char* realtorGetCompanyName(Realtor realtor);

int realtorGetTaxPercentage(Realtor realtor);

Map realtorGetOffers(Realtor realtor);

Map realtorGetServices(Realtor realtor);

int realtorGetTotalApartmentNumber(Realtor realtor);

int realtorGetAverageMedianPrice(Realtor realtor);

int realtorGetAverageMedianArea(Realtor realtor);

#endif /* REALTOR_H_ */
