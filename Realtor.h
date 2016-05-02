/*
 * Realtor.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#ifndef REALTOR_H_
#define REALTOR_H_

#include "yad3service.h"
#include "Customer.h"	// <<<< might be because of double including


typedef struct Realtor_t* Realtor;



Realtor realtorCreate(char* company_name, int tax_percentage);

void realtorDestroy(Realtor realtor);

yad3Result realtorAddService
		(Realtor realtor, char* service_name, int max_apartments);

yad3Result realtorRemoveService(Realtor realtor, char* service_name);

yad3Result realtorAddApartment(Realtor realtor, char* service_name,
		int id, int price, int width, int height, char* matrix);

yad3Result realtorRemoveApartment(Realtor realtor, char* service_name, int id);

yad3Result realtorRespondToOffer
		(Realtor realtor, char* customer_email, Choice choice);

bool realtorIsRealtorRelevant(Realtor realtor, Customer customer);

char* realtorGetCompanyName(Realtor realtor);

int realtorGetTaxPercentage(Realtor realtor);

Map realtorGetOffers(Realtor realtor);

Map realtorGetServices(Realtor realtor);

#endif /* REALTOR_H_ */
