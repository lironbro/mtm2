/*
 * yad3service.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#include "yad3service.h"




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

static void str_free(char* str){
	free(str);
}


// not static, but still useful both in yad3service and customer
bool apartmentCorrectProperties(Customer customer, Apartment apartment){
	return apartmentGetPrice(apartment) <= customerGetMaxPrice(customer)
			&& apartmentTotalArea(apartment) <= customerGetMinArea(customer)
			&& apartmentNumOfRooms(apartment) <= customerGetMinRooms(customer);
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
	yad3->userEmails=mapCreate(copyDataElement, copyKeyElement, freeDataElement, freeKeyElement, compareKeyElements);
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

yad3Result yad3serviceAddRealtor(yad3service yad3,
		char* email, char* company_name, int tax_percentage)
{
	if(yad3 == NULL || yad3->userEmails == NULL ||
			!emailIsValid(email) || !intIsInRange(tax_percentage, 1, 100))
		return YAD3_INVALID_PARAMETERS;
	if(mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_ALREADY_EXISTS;
	User realtor = userRealtorCreate(company_name, tax_percentage);
	return YAD3_SUCCESS;
}

yad3Result yad3serviceRemoveRealtor(yad3service yad3, char* email){
	if(yad3 == NULL || yad3->userEmails == NULL || !emailIsValid(email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	yad3Result result = userRealtorDestroy(mapGet(yad3->userEmails, email));
	if(result == YAD3_SUCCESS)
		mapRemove(yad3->userEmails, email);
	return result;
}

yad3Result yad3serviceRealtorAddApartmentService
		(yad3service yad3, char* email, char* service_name, int max_apartments){
	if(yad3 == NULL || !emailIsValid(email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return userRealtorAddService(mapGet(yad3->userEmails, email),
			service_name, max_apartments);
}

yad3Result yad3serviceRealterRemoveApartmentService
		(yad3service yad3, char* email, char* service_name){
	if(yad3 == NULL || !emailIsValid(email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return userRealtorRemoveService(mapGet(yad3->userEmails, email),
			service_name);
}

yad3Result yad3serviceRealtorAddApartment
		(yad3service yad3, char* email, char* service_name,
				int id, int price, int width, int height, char* matrix){
	if(yad3 == NULL || !emailIsValid(email) || id < 0)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return userRealtorAddApartment(mapGet(yad3->userEmails, email),
			service_name, id, price, width, height, matrix);
}

yad3Result yad3serviceRealtorRemoveApartment
		(yad3service yad3, char* email, char* service_name, int id){
	if(yad3 == NULL || !emailIsValid(email) || id < 0)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return userRealtorRemoveApartment(mapGet(yad3->userEmails, email),
			service_name, id);
}

yad3Result yad3serviceAddCustomer
		(yad3service yad3, char* email, int min_area, int min_rooms, int max_price){
	if(yad3 == NULL || !emailIsValid(email) || min_area <= 0
			|| min_rooms <= 0 || max_price <= 0)
		return YAD3_INVALID_PARAMETERS;
	if(mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_ALREADY_EXISTS;
	User user = userCustomerCreate(min_area, min_rooms, max_price);
	mapPut(yad3->userEmails, email, user);
	return YAD3_SUCCESS;
}

yad3Result yad3serviceRemoveCustomer(yad3service yad3, char* email){
	if(yad3 == NULL || !emailIsValid(email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	yad3Result result = userCustomerDestroy(mapGet(yad3->userEmails, email));
	if(result == YAD3_SUCCESS){
		mapRemove(yad3->userEmails, email);
	}
	return result;
}

yad3Result yad3serviceCustomerPurchase
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(realtor_email) || apartment_id < 0)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails, realtor_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return userCustomerPurchase(mapGet(yad3->userEmails, email),
			mapGet(yad3->userEmails, realtor_email), service_name, apartment_id);
}

yad3Result yad3serviceCustomerMakeOffer
		(yad3service yad3, char* email, char* realtor_email,
				char* service_name, int apartment_id, int new_price){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(realtor_email) || apartment_id < 0)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails, realtor_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return userCustomerMakeOffer(mapGet(yad3->userEmails, email),
			mapGet(yad3->userEmails, realtor_email), apartment_id, new_price,
					service_name, email);
}

yad3Result yad3serviceRealtorRespondToOffer
		(yad3service yad3, char* email, char* customer_email, Choice choice){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(customer_email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails, customer_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return userRealtorRespondToOffer(mapGet(yad3->userEmails, email),
			mapGet(yad3->userEmails, customer_email), customer_email, choice);
}


yad3Result yad3serviceReportRelevantRealtors(yad3service yad3, char* customer_email){
	if(yad3 == NULL || yad3->userEmails == NULL)
		return YAD3_INVALID_PARAMETERS;	// need better error type here
	if(!mapContains(yad3->userEmails, customer_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	User user = mapGet(yad3->userEmails, customer_email);
	if(userGetType(user) != customer){
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	Customer customer = (Customer*)(user->data);	// <<<< check this casting
	List relevant_emails = listCreate(str_copy, str_free);	// <<<needs checking
	Map emails = yad3->userEmails;
	for(char* iterator = mapGetFirst(emails);
			iterator != NULL; iterator = mapGetNext(emails)){
		if(userIsRealtorRelevant(mapGet(emails, iterator), customer))
			listInsertLast(relevant_emails, iterator);
	}
	listSort(relevant_emails);	// <<<<<<<<<<< need to sort the result
	// <<<<<<<<<<<<<<<< print somehow, probably using given print function
	return YAD3_SUCCESS;
}


// ------------------- </yad3service> -------------------

