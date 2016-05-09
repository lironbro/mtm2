/*
 * yad3service.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#include "yad3service.h"
#include <string.h>



// ------------------- <General Static functions> -------------------

/*
 * emailIsValid: checks if the email is valid, meaning it contains a '@'
 */
static bool emailIsValid(char* email){
	bool one_present = false;
	while(*email){
		if(*email == '@' && !one_present){
			one_present = true;
		}
		else if(*email == '@' && one_present){
			return false;
		}
		email++;
	}
	return one_present;
}

/*
 * intIsInRange: checks if num is between min and max (inclusive)
 */
static bool intIsInRange(int num, int min, int max){
	return num>=min && num<= max;
}

/*
 * strFree: frees a string
 */
static void strFree(char* str){
	free(str);
}

/*
 * getBestCustomerPayment: returns the customer who has payed the most
 */
static int getBestCustomerPayment(yad3service yad3){
	int max = -1;
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

/*
 * realtorCalculateScore: calculates the realtor's score by the given formula
 */
static int realtorCalculateScore(Realtor realtor){
	return 1000000*realtorGetTotalApartmentNumber(realtor)+
			realtorGetAverageMedianPrice(realtor)+
			100000*realtorGetAverageMedianArea(realtor);
}

/*
 * getBestRealtorScore: returns the realtor with the biggest score
 */
static int getBestRealtorScore(yad3service yad3){
	int max = -1;
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

/*
 * resultUserToYad3: converts a userResult to the corresponding yad3Result
 */
static yad3Result resultUserToYad3(userResult result){
	switch(result){
	case USER_INVALID_PARAMETERS:
		return YAD3_INVALID_PARAMETERS;
		break;
	case USER_EMAIL_ALREADY_EXISTS:
		return YAD3_EMAIL_ALREADY_EXISTS;
		break;
	case USER_EMAIL_DOES_NOT_EXIST:
		return YAD3_EMAIL_DOES_NOT_EXIST;
		break;
	case USER_EMAIL_WRONG_ACCOUNT_TYPE:
		return USER_EMAIL_WRONG_ACCOUNT_TYPE;
		break;
	case USER_APARTMENT_SERVICE_ALREADY_EXISTS:
		return USER_APARTMENT_SERVICE_ALREADY_EXISTS;
		break;
	case USER_APARTMENT_SERVICE_DOES_NOT_EXIST:
		return USER_APARTMENT_SERVICE_DOES_NOT_EXIST;
		break;
	case USER_APARTMENT_SERVICE_FULL:
		return USER_APARTMENT_SERVICE_FULL;
		break;
	case USER_APARTMENT_ALREADY_EXISTS:
		return USER_APARTMENT_ALREADY_EXISTS;
		break;
	case USER_APARTMENT_DOES_NOT_EXIST:
		return USER_APARTMENT_DOES_NOT_EXIST;
		break;
	case USER_PURCHASE_WRONG_PROPERTIES:
		return USER_PURCHASE_WRONG_PROPERTIES;
		break;
	case USER_ALREADY_REQUESTED:
		return USER_ALREADY_REQUESTED;
		break;
	case USER_REQUEST_WRONG_PROPERTIES:
		return USER_REQUEST_WRONG_PROPERTIES;
		break;
	case USER_REQUEST_ILLOGICAL_PRICE:
		return USER_REQUEST_ILLOGICAL_PRICE;
		break;
	case USER_NOT_REQUESTED:
		return USER_NOT_REQUESTED;
		break;
	case USER_OUT_OF_MEMORY:
		return USER_OUT_OF_MEMORY;
		break;
	case USER_NULL_ARGUMENT:
		return USER_NULL_ARGUMENT;
		break;
	case USER_REQUEST_WRONG_PPROPERTIES:
		return USER_REQUEST_WRONG_PPROPERTIES;
		break;
	default: return YAD3_SUCCESS;
	}
}

/*
 * yad3RemoveUser: removes the user with the given email and type from the
 * 			yad3 service
 *
 * @param yad3: the service from which the user will be removed
 * @param email: the email address of the user
 * @param type: the type of the user
 *
 * returns:
 * YAD3_SUCCESS if the user has been removed successfully
 * YAD3_INVALID_PARAMETERS if a wrong parameter has been sent
 * YAD3_EMAIL_DOES_NOT_EXIST if there is no user with the given email address
 * YAD3_EMAIL_WRONG_ACCOUNT_TYPE if the user with the given address is of
 * 			a different type
 */
static yad3Result yad3RemoveUser(yad3service yad3, char* email, userType type){
	if(yad3 == NULL || yad3serviceGetEmails(yad3) == NULL ||
			!emailIsValid(email))
		return YAD3_INVALID_PARAMETERS;
	Map emails = yad3serviceGetEmails(yad3);
	if(!mapContains(emails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	if(userGetType(mapGet(emails, email)) != type)
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	mapRemove(emails, email);
	return YAD3_SUCCESS;
}

/*
 * strCopy: copies a string and returns the copy
 */
static char* strCopy(const char* str){
	int len = strlen(str);
	char* result = malloc(sizeof(char*)*(len+1));
	if(result==NULL){
		return NULL;
	}
	memset(result, '\0', len+1);
	strcpy(result, str);
	return result;
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
			(copyMapKeyElements)strCopy, (freeMapDataElements)userDestroy,
			(freeMapKeyElements)strFree, (compareMapKeyElements)strcmp);
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
	userDestroy(realtor);
	return YAD3_SUCCESS;
}

yad3Result yad3serviceRemoveRealtor(yad3service yad3, char* email){
	return yad3RemoveUser(yad3, email, realtor);
}

yad3Result yad3serviceRealtorAddApartmentService
(yad3service yad3, char* email, char* service_name, int max_apartments){
	if(yad3 == NULL || !emailIsValid(email) || max_apartments <= 0)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return resultUserToYad3(userRealtorAddService(mapGet(yad3->userEmails,
			email), service_name, max_apartments));
}

yad3Result yad3serviceRealterRemoveApartmentService
(yad3service yad3, char* email, char* service_name){
	if(yad3 == NULL || !emailIsValid(email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return resultUserToYad3(userRealtorRemoveService(mapGet(yad3->userEmails,
			email), service_name));
}

yad3Result yad3serviceRealtorAddApartment
(yad3service yad3, char* email, char* service_name,
		int id, int price, int width, int height, char* matrix){
	if(yad3 == NULL || !emailIsValid(email) || id < 0 || matrix == NULL)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return resultUserToYad3(userRealtorAddApartment(mapGet(yad3->userEmails,
			email), service_name, id, price, width, height, matrix));
}

yad3Result yad3serviceRealtorRemoveApartment
(yad3service yad3, char* email, char* service_name, int id){
	if(yad3 == NULL || !emailIsValid(email) || id < 0)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return resultUserToYad3(userRealtorRemoveApartment(mapGet(yad3->userEmails,
			email), service_name, id));
}

yad3Result yad3serviceAddCustomer
(yad3service yad3, char* email, int min_area, int min_rooms,
		int max_price){
	if(yad3 == NULL || !emailIsValid(email) || min_area <= 0
			|| min_rooms <= 0 || max_price <= 0)
		return YAD3_INVALID_PARAMETERS;
	if(mapContains(yad3->userEmails, email))
		return YAD3_EMAIL_ALREADY_EXISTS;
	User user = userCustomerCreate(min_area, min_rooms, max_price);
	mapPut(yad3->userEmails, email, user);
	userDestroy(user);
	return YAD3_SUCCESS;
}

yad3Result yad3serviceRemoveCustomer(yad3service yad3, char* email){
	return yad3RemoveUser(yad3, email, customer);
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
	return resultUserToYad3(userCustomerPurchase(mapGet(yad3->userEmails,
			email), mapGet(yad3->userEmails, realtor_email),
			service_name, apartment_id));
}

yad3Result yad3serviceCustomerMakeOffer
(yad3service yad3, char* email, char* realtor_email,
		char* service_name, int apartment_id, int new_price){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(realtor_email) || apartment_id < 0 || new_price <= 0)
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails, realtor_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return resultUserToYad3(userCustomerMakeOffer(mapGet(yad3->userEmails,
			email), mapGet(yad3->userEmails, realtor_email), email,
			service_name, apartment_id, new_price));
}

yad3Result yad3serviceRealtorRespondToOffer
(yad3service yad3, char* email, char* customer_email, Choice choice){
	if(yad3 == NULL || !emailIsValid(email) ||
			!emailIsValid(customer_email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, email)
			|| !mapContains(yad3->userEmails, customer_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	return resultUserToYad3(userRealtorRespondToOffer(mapGet(yad3->userEmails,
			email), mapGet(yad3->userEmails, customer_email),
			customer_email, choice));
}


yad3Result yad3serviceReportRelevantRealtors(yad3service yad3,
		char* customer_email, FILE* output){
	if(yad3 == NULL || yad3->userEmails == NULL ||
			!emailIsValid(customer_email))
		return YAD3_INVALID_PARAMETERS;
	if(!mapContains(yad3->userEmails, customer_email))
		return YAD3_EMAIL_DOES_NOT_EXIST;
	User user = mapGet(yad3->userEmails, customer_email);
	if(userGetType(user) != customer){
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	Customer customer = (Customer)(userGetData(user));
	List relevant_emails = listCreate((CopyListElement)strCopy,
			(FreeListElement)strFree);
	Map emails = yad3->userEmails;

	MAP_FOREACH(char*, iterator, emails){
		if(userIsRealtorRelevant(mapGet(emails, iterator), customer))
			listInsertLast(relevant_emails, iterator);	// <<<<< TURNS TO NULL???
	}

	listSort(relevant_emails, (CompareListElements)strcmp);

	LIST_FOREACH(char*, current, relevant_emails){
		Realtor realtor = (Realtor)userGetData(mapGet(yad3->userEmails, current));
		mtmPrintRealtor(output, current, realtorGetCompanyName(realtor));
	}
	listDestroy(relevant_emails);
	return YAD3_SUCCESS;
}

yad3Result yad3serviceReportMostPayingCustomers(yad3service yad3, int count,
		FILE* output){
	if(yad3 == NULL || count <= 0)
		return YAD3_INVALID_PARAMETERS;
	Map best_customers = mapCreate((copyMapDataElements)customerCopy,
			(copyMapKeyElements)strCopy, (freeMapDataElements)customerDestroy,
			(freeMapKeyElements)strFree, (compareMapKeyElements)strcmp);
	List best_emails = listCreate((CopyListElement)strCopy,
			(FreeListElement)strFree);
	for(mapGetFirst(yad3->userEmails); count > 0; count--){
		int max = getBestCustomerPayment(yad3);
		if(max == -1)
			break;
		MAP_FOREACH(char*, iterator, yad3->userEmails){
			User user = mapGet(yad3->userEmails, iterator);
			if(userGetType(user) == realtor)
				continue;
			Customer customer = (Customer)userGetData(user);
			if(customerGetTotalPayment(customer) == max){
				mtmPrintCustomer(output, iterator,
						customerGetTotalPayment(customer));
				listInsertLast(best_emails, iterator);
				mapPut(best_customers, iterator, customer);
				mapRemove(yad3->userEmails, iterator);
				break;
			}
		}
	}
	listSort(best_emails, (CompareListElements)strcmp);

	LIST_FOREACH(char*, iterator, best_emails){
		User customer = userCustomerCopy(
				mapGet(best_customers, listGetCurrent(best_emails)));
		mapPut(yad3->userEmails, iterator, customer);
	}
	mapDestroy(best_customers);
	listDestroy(best_emails);
	return YAD3_SUCCESS;
}

yad3Result yad3serviceReportSignificantRealtors(yad3service yad3, int count,
		FILE* output){
	if(yad3 == NULL || count <= 0)
		return YAD3_INVALID_PARAMETERS;
	Map best_realtors = mapCreate((copyMapDataElements)realtorCopy,
			(copyMapKeyElements)strCopy, (freeMapDataElements)realtorDestroy,
			(freeMapKeyElements)strFree, (compareMapKeyElements)strcmp);
	List best_emails = listCreate((CopyListElement)strCopy,
		(FreeListElement)strFree);
	for(mapGetFirst(yad3->userEmails); count > 0; count--){
		int max = getBestRealtorScore(yad3);
		if(max == -1)
			break;
		MAP_FOREACH(char*, iterator, yad3->userEmails){
			User user = mapGet(yad3->userEmails, iterator);
			if(userGetType(user)!= realtor)
				continue;
			Realtor realtor = (Realtor)userGetData(user);
			if(realtorCalculateScore(realtor) == max){
				listInsertLast(best_emails, iterator);
				mtmPrintRealtor(output, iterator,
						realtorGetCompanyName(realtor));
				mapPut(best_realtors, iterator, realtor);
				mapRemove(yad3->userEmails, iterator);
				break;
			}
		}
	}
	LIST_FOREACH(char*, iterator, best_emails){
		User realtor = userRealtorCopy(mapGet(
				best_realtors, listGetCurrent(best_emails)));
		mapPut(yad3->userEmails, iterator, realtor);
	}
	mapDestroy(best_realtors);
	listDestroy(best_emails);
	return YAD3_SUCCESS;
}

// ------------------- </yad3service> -------------------

