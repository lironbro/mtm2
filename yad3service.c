/*
 * yad3service.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#include "yad3service.h"
#include <string.h>



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

static void str_free(char* str){
	free(str);
}


static int getBestCustomer(yad3service yad3){
	int max = 0;
	MAP_FOREACH(char*, current, yad3serviceGetEmails(yad3)){
		User user = mapGet(yad3serviceGetEmails(yad3), current);
		if(userGetType(user) == realtor)
			continue;
		Customer customer = (Customer)userGetData(user);
		if(customerGetTotalPayment(customer) >= max)
			max = customerGetTotalPayment(customer);
	}
	return max;

}

static int realtorCalculateScore(Realtor realtor){
	return 1000000*realtorGetTotalApartmentNumber(realtor)+
			realtorGetAverageMedianPrice(realtor)+
			100000*realtorGetAverageMedianArea(realtor);
}

static int getBestRealtor(yad3service yad3){
	int max = 0;
	MAP_FOREACH(char*, current, yad3serviceGetEmails(yad3)){
		User user = mapGet(yad3serviceGetEmails(yad3), current);
		if(userGetType(user) == customer)
			continue;
		Realtor realtor = (Realtor)userGetData(user);
		if(realtorCalculateScore(realtor) >= max){
			max = realtorCalculateScore(realtor);
		}
	}
	return max;
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
yad3service yad3serviceCreate()
{
	yad3service yad3 = malloc(sizeof(*yad3));
	if(yad3 == NULL){
		return NULL;
	}
	yad3->userEmails = mapCreate((copyMapDataElements)userCopy,
			(copyMapKeyElements)strcpy, (freeMapDataElements)userDestroy,
			(freeMapKeyElements)str_free, (compareMapKeyElements)strcmp);
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

Map yad3serviceGetEmails(yad3service yad3){
	return yad3->userEmails;
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
	mapPut(yad3->userEmails, email, realtor);
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
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, customer_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	User user = mapGet(yad3->userEmails, customer_email);
	if(userGetType(user) != customer){
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	Customer customer = (Customer)(userGetData(user));	// <<<< check this casting
	List relevant_emails = listCreate((CopyListElement)strcpy,
			(FreeListElement)str_free);	// <<<needs checking
	Map emails = yad3->userEmails;
	for(char* iterator = mapGetFirst(emails);
			iterator != NULL; iterator = mapGetNext(emails)){
		if(userIsRealtorRelevant(mapGet(emails, iterator), customer))
			listInsertLast(relevant_emails, iterator);
	}
	FILE* output = fopen("output.txt", "w");
	listSort(relevant_emails, (CompareListElements)strcmp);
	for(listGetFirst(relevant_emails); listGetCurrent(relevant_emails) != NULL;
			listGetNext(relevant_emails)){
		Realtor realtor = mapGet(yad3->userEmails,
				listGetCurrent(relevant_emails));
		mtmPrintRealtor(output, listGetCurrent(relevant_emails),
				realtorGetCompanyName(realtor));
		realtorDestroy(realtor);
	}
	return YAD3_SUCCESS;
}

// should work, other than printing equal customers by address
yad3Result yad3serviceReportMostPayingCustomers(yad3service yad3, int count){
	if(yad3 == NULL || count <= 0)
		return YAD3_INVALID_PARAMETERS;
	Map best_customers = mapCreate((copyMapDataElements)customerCopy,
			(copyMapKeyElements)strcpy, (freeMapDataElements)customerDestroy,
			(freeMapKeyElements)str_free, (compareMapKeyElements)strcmp);
	FILE* output = fopen("output.txt", "w");	// will be written here (?)
	for(mapGetFirst(yad3->userEmails); count > 0; count--){
		int max = getBestCustomer(yad3);
		if(max == 0)
			break;
		MAP_FOREACH(char*, j, yad3->userEmails){
			User user = mapGet(yad3->userEmails, j);
			if(userGetType(user) == realtor)
				continue;
			Customer customer = (Customer)userGetData(user);
			if(customerGetTotalPayment(customer) == max){
				mtmPrintCustomer(output, j, customerGetTotalPayment(customer));
				mapPut(best_customers, j, customer);
				mapRemove(yad3->userEmails, j);
				break;
			}
		}
	}
	MAP_FOREACH(char*, i, best_customers){
		mapPut(yad3->userEmails, i, mapGet(best_customers, i));
	}
	mapDestroy(best_customers);
	return YAD3_SUCCESS;
}

// should work, other than printing equal customers by address
yad3Result yad3serviceReportSignificantRealtors(yad3service yad3, int count){
	if(yad3 == NULL || count <= 0)
		return YAD3_INVALID_PARAMETERS;
	FILE* output = fopen("output.txt", "w");
	Map best_realtors = mapCreate((copyMapDataElements)realtorCopy,
			(copyMapKeyElements)strcpy, (freeMapDataElements)realtorDestroy,
			(freeMapKeyElements)str_free, (compareMapKeyElements)strcmp);
	for(mapGetFirst(yad3->userEmails); count > 0; count--){
		int max = getBestRealtor(yad3);
		if(max == 0)
			break;
		MAP_FOREACH(char*, j, yad3->userEmails){
			User user = mapGet(yad3->userEmails, j);
			if(userGetType(user)!= realtor)
				continue;
			Realtor realtor = (Realtor)userGetData(user);
			if(realtorCalculateScore(realtor) == max){
				mtmPrintRealtor(output, j, realtorGetCompanyName(realtor));	// <<<< define file
				mapPut(best_realtors, j, realtor);
				mapRemove(yad3->userEmails, j);
				break;
			}
		}
	}
	MAP_FOREACH(char*, i, best_realtors){
		mapPut(yad3->userEmails, i, mapGet(best_realtors, i));
	}
	mapDestroy(best_realtors);
	return YAD3_SUCCESS;
}

// ------------------- </yad3service> -------------------

