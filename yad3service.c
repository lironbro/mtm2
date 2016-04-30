/*
 * yad3service.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#include "yad3service.h"


// ------------------- <Offer> -------------------

typedef struct offer_t{
	int id;
	int price;
	char* service_name;
} *Offer;

static Offer offerCreate(int id, int price, char* service_name){
	Offer offer = malloc(sizeof(*offer));
	offer->id = id;
	offer->price = price;
	offer->service_name = str_copy(service_name, malloc(sizeof(char)));
	return offer;
}

static void offerDestroy(Offer offer){
	free(offer->service_name);
	free(offer);
}

// ------------------- </Offer> -------------------



// ------------------- <General Static functions> -------------------

static bool emailIsValid(char* email){
	while(*email){
		if(*email == '@'){
			return true;
		}
		email++;
	}
	return false;
}

static bool intIsInRange(int num, int min, int max){
	return num>=min && num<= max;
}

static int str_length(char* str){
	if(str == NULL)
		return -1;
	int count = 0;
	while(*str){
		str++;
		count++;
	}
	return count;
}

static char* str_copy(char* source, char* destination){
	free(destination);	// not sure if this is legal
	destination = malloc(sizeof(char)*(1+str_length(source)));
	if(destination == NULL){
		return NULL;
	}
	int i = 0;
	while(*(source+i)){
		*(destination+i) = *(source+i);
		i++;
	}
	*(destination+i) = *(source+i);	// places the '/0' at the end, needs checking
	return destination;
}


// not static, but still useful both in yad3service and customer
bool apartmentCorrectProperties(Customer customer, Apartment apartment){
	return apartmentGetPrice(apartment) <= customer->max_price
			&& apartmentTotalArea(apartment) <= customer->min_area
			&& apartmentNumOfRooms(apartment) <= customer->min_rooms;
}
// ------------------- </General Static functions> -------------------

// ------------------- <yad3service> -------------------

struct yad3service_t {
	Map userEmails;
};

// I recommend calling every instance of yad3service "yad3" or something similar
// just not "service" because that could also be interpreted as an instance of
// apartmentService, which could be dangerous in some functions

// link copy & delete functions for char* and user later
yad3service yad3serviceCreate(copyMapDataElements copyDataElement, copyMapKeyElements copyKeyElement,
		freeMapDataElements freeDataElement, freeMapKeyElements freeKeyElement,
		compareMapKeyElements compareKeyElements)
{
	yad3service yad3 = malloc(sizeof(*yad3));
	if(yad3 == NULL){
		return NULL;
	}
	yad3->userEmails=mapCreate(copyDataElement, copyKeyElement, freeDataElement,
			freeMapKeyElements, compareKeyElements);
	if(yad3->userEmails == NULL){
		return NULL;
	}
	return yad3;
}

void yad3serviceDestroy(yad3service yad3)
{
	mapDestroy(yad3->userEmails);
	free(yad3);
}

MTMResult yad3serviceAddRealtor(yad3service yad3,
		char* email, char* company_name, int tax_percentage)
{
	if(yad3 == NULL || yad3->userEmails == NULL ||
			!emailIsValid(email) || !intIsInRange(tax_percentage, 1, 100))
		return MTM_INVALID_PARAMETERS;
	if(mapContains(yad3->userEmails, email))
		return MTM_EMAIL_ALREADY_EXISTS;
	User realtor = userRealtorCreate()
	return MTM_SUCCESS;
}

MTMResult yad3serviceRemoveRealtor(yad3service yad3, char* email){
	if(yad3 == NULL || yad3->userEmails == NULL || !emailIsValid(email))
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return MTM_EMAIL_DOES_NOT_EXIST;
	MTMResult result = userRealtorDestroy(mapGet(yad3->userEmails, email));
	if(result == MTM_SUCCESS)
		mapRemove(yad3->userEmails, email);
	return result;
}

MTMResult yad3serviceRealtorAddApartmentService
		(yad3service yad3, char* email, char* service_name, int max_apartments){
	if(yad3 == NULL || !emailIsValid(email))
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return MTM_EMAIL_DOES_NOT_EXIST;
	return userRealtorAddService(mapGet(yad3->userEmails, email),
			service_name, max_apartments);
}

MTMResult yad3serviceRealterRemoveApartmentService
		(yad3service yad3, char* email, char* service_name){
	if(yad3 == NULL || !emailIsValid(email))
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return MTM_EMAIL_DOES_NOT_EXIST;
	return userRealtorRemoveService(mapGet(yad3->userEmails, email),
			service_name);
}

MTMResult yad3serviceRealtorAddApartment
		(yad3service yad3, char* email, char* service_name,
				int id, int price, int width, int height, char* matrix){
	if(yad3 == NULL || !emailIsValid(email) || id < 0)
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return MTM_EMAIL_DOES_NOT_EXIST;
	return userRealtorAddApartment(mapGet(yad3->userEmails, email),
			service_name, id, price, width, height, matrix);
}

MTMResult yad3serviceRealtorRemoveApartment
		(yad3service yad3, char* email, char* service_name, int id){
	if(yad3 == NULL || !emailIsValid(email) || id < 0)
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return MTM_EMAIL_DOES_NOT_EXIST;
	return userRealtorRemoveApartment(mapGet(yad3->userEmails, email),
			service_name, id);
}

MTMResult yad3serviceAddCustomer
		(yad3service yad3, char* email, int min_area, int min_rooms, int max_price){
	if(yad3 == NULL || !emailIsValid(email) || min_area <= 0
			|| min_rooms <= 0 || max_price <= 0)
		return MTM_INVALID_PARAMETERS;
	if(mapContains(yad3->userEmails, email))
		return MTM_EMAIL_ALREADY_EXISTS;
	User user = userCustomerCreate(min_area, min_rooms, max_price);
	mapPut(yad3->userEmails, email, user);
	return MTM_SUCCESS;
}

MTMResult yad3serviceRemoveCustomer(yad3service yad3, char* email){
	if(yad3 == NULL || !emailIsValid(email))
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return MTM_EMAIL_DOES_NOT_EXIST;
	MTMResult result = userCustomerDestroy(mapGet(yad3->userEmails, email));
	if(result == MTM_SUCCESS){
		mapRemove(yad3->userEmails, email);
	}
	return result;
}

MTMResult yad3serviceCustomerPurchase
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(realtor_email) || apartment_id < 0)
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails), realtor_email)
		return MTM_EMAIL_DOES_NOT_EXIST;
	return userCustomerPurchase(mapGet(yad3->userEmails, email),
			mapGet(yad3->userEmails, realtor_email), service_name, apartment_id);
}

MTMResult yad3serviceCustomerMakeOffer
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id, int new_price){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(realtor_email) || apartment_id < 0)
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails), realtor_email)
		return MTM_EMAIL_DOES_NOT_EXIST;
	return userCustomerMakeOffer(mapGet(yad3->userEmails, email),
			mapGet(yad3->userEmails, realtor_email, id, price,
					service_name, email));
}

MTMResult yad3serviceRealtorRespondToOffer
		(yad3service yad3, char* email, char* customer_email, Choice choice){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(customer_email))
		return MTM_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails), customer_email)
		return MTM_EMAIL_DOES_NOT_EXIST;
	return userRealtorRespondToOffer(mapGet(yad3->userEmails, email),
			mapGet(yad3->userEmails, customer_email), customer_email, choice);
}


// ------------------- </yad3service> -------------------

