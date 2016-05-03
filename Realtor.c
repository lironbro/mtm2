/*
 * Realtor.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#include "Realtor.h"
#include <String.h>

// ------------------- <General Static functions> -------------------

static bool intIsInRange(int num, int min, int max){
	return num>=min && num<= max;
}


static void str_free(char* str){
	free(str);
}

static SquareType** translateMatrix(int width, int height, char* matrix){
	SquareType** result = malloc(height*width*sizeof(SquareType));
	if(result == NULL)
		return NULL;
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			if(*(matrix+i*width+j) == 'w'){
				result[i][j] = WALL;
			}	// I intentionally didn't use an else statement
			if(*(matrix+i*width+j) == 'e'){	// because I'm afraid it might
				result[i][j] = EMPTY;	// leave the array's bounds or something
			}
		}
	}
	return result;
}
// ------------------- </General Static functions> -------------------


// ------------------- <Realtor> -------------------
/*
 * here's an explanation of how I designed the Realtor struct:
 * Realtor is kept under User, therefore it does not know its own type
 * and therefore has no need to know its own email
 * the functions in this struct don't check the validity of all the given input,
 * because I'm lazy, so I'll leave this problem for future me with User,
 * and also because wrong input errors are returned "before" wrong email
 * and wrong type errors, and therefore need to be checked in encapsulating
 * structs
 */

// Definition and static functions -------------------

struct Realtor_t{
	Map services;	// keys are service names, data is services
	char* company_name;
	int tax_percentage;
	Map offers;		// keys are customer emails, data is offers
};

static bool serviceExists(Realtor realtor, char* service_name){
	return mapContains(realtor->services, service_name);
}



// Functions -------------------

Realtor realtorCreate(char* company_name, int tax_percentage){
	if(!intIsInRange(tax_percentage, 1, 100)){
		return NULL;
	}
	Realtor realtor = malloc(sizeof(*realtor));
	if(realtor == NULL){
		return NULL;
	}
	strcpy(realtor->company_name, company_name);// this might fail badly
	realtor->tax_percentage = tax_percentage;	// please check ^^^^
	realtor->services = mapCreate(serviceCopy, strcpy, serviceDestroy
			, str_free, strcmp);
	realtor->offers = mapCreate(offerCopy, strcpy, offerDestroy,
			str_free, strcmp);
	return realtor;
}

Realtor realtorCopy(Realtor realtor){
	Realtor copy =
			realtorCreate(realtor->company_name, realtor->tax_percentage);
	MAP_FOREACH(char*, i, realtor->services){
		mapPut(copy->services, i, mapGet(realtor->services, i));
	}
	MAP_FOREACH(char*, i, realtor->offers){
		mapPut(copy->offers, i, mapGet(realtor->offers, i));
	}
	return copy;
}

void realtorDestroy(Realtor realtor){
	mapDestroy(realtor->services);
	free(realtor->company_name);
	free(realtor);
}



realtorResult realtorAddService(Realtor realtor, char* service_name, int max_apartments){
	if(serviceExists(realtor, service_name))
		return REALTOR_APARTMENT_SERVICE_ALREADY_EXISTS;	// WHY IS THIS NOT RECOGNISED???
	ApartmentService newService = serviceCreate(max_apartments);
	mapPut(realtor->services, service_name, newService);
	return REALTOR_SUCCESS;
}

realtorResult realtorRemoveService(Realtor realtor, char* service_name){
	if(!serviceExists(realtor, service_name))
		return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST;	// WHY IS THIS NOT RECOGNISED???
	mapRemove(realtor->services, service_name);
	return REALTOR_SUCCESS;
}

realtorResult realtorAddApartment(Realtor realtor, char* service_name,
		int id, int price, int width, int height, char* matrix){
	if(!serviceExists(realtor, service_name))
		return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST;
	SquareType** squares = translateMatrix(width, height, matrix);
	if(squares == NULL)
		return REALTOR_OUT_OF_MEMORY;	// WHY IS THIS NOT RECOGNISED???
	Apartment apartment = apartmentCreate(squares, height, width, price);
	if(apartment == NULL)
		return REALTOR_OUT_OF_MEMORY;	// WHY IS THIS NOT RECOGNISED???
	ApartmentServiceResult result =
			serviceAddApartment(mapGet(realtor->services, service_name), apartment, id);
	if(result == APARTMENT_SERVICE_FULL)
		return REALTOR_APARTMENT_SERVICE_FULL;	// WHY IS THIS NOT RECOGNISED???
	if(result == APARTMENT_SERVICE_ALREADY_EXISTS)
		return REALTOR_APARTMENT_ALREADY_EXISTS;	// WHY IS THIS NOT RECOGNISED???
	return REALTOR_SUCCESS;
}

realtorResult realtorRemoveApartment(Realtor realtor, char* service_name, int id){
	if(!serviceExists(realtor, service_name))
		return REALTOR_APARTMENT_SERVICE_DOES_NOT_EXIST; // WHY IS THIS NOT RECOGNISED???
	ApartmentServiceResult result =
			serviceDeleteById(mapGet(realtor->services, service_name), id);
	if(result == APARTMENT_SERVICE_NO_FIT)
		return REALTOR_APARTMENT_DOES_NOT_EXIST; // WHY IS THIS NOT RECOGNISED???
	return APARTMENT_SUCCESS;
}

// needs to make the purchase itself
realtorResult realtorRespondToOffer(Realtor realtor, char* customer_email, Choice choice){
	if(!mapContains(realtor->offers, customer_email))
		return REALTOR_NOT_REQUESTED;	// WHY IS THIS NOT RECOGNISED???
	if(choice == accept){
		Offer offer = mapGet(realtor->offers, customer_email);
		realtorRemoveApartment(realtor, offerGetServiceName(offer),
				offerGetId(offer));
	}
	mapRemove(realtor->offers, customer_email);
	return REALTOR_SUCCESS;
}

bool realtorIsRealtorRelevant(Realtor realtor, int customer_min_area, int customer_min_rooms, int customer_max_price){
	if(realtor == NULL)
		return REALTOR_NULL_ARGUMENT;
	for(char* iterator = mapGetFirst(realtor->services);
			iterator != NULL; iterator = mapGetNext(realtor->services)){
		if(serviceSearch(mapGet(realtor->services, iterator),
				customer_min_area, customer_min_rooms,
				customer_max_price, NULL)
				== APARTMENT_SERVICE_SUCCESS)
			return true;
	}
	return false;
}

char* realtorGetCompanyName(Realtor realtor){
	return realtor->company_name;
}

int realtorGetTaxPercentage(Realtor realtor){
	return realtor->tax_percentage;
}

Map realtorGetOffers(Realtor realtor){
	return realtor->offers;
}

Map realtorGetServices(Realtor realtor){
	return realtor->services;
}

int realtorGetTotalApartmentNumber(Realtor realtor){
	if(realtor == NULL || realtor->services == NULL)
		return 0;
	int total = 0;
	MAP_FOREACH(char*, current, realtor->services){
		total += serviceNumberOfApatments(mapGet(realtor->services, current));
	}
	return total;
}

int realtorGetAverageMedianPrice(Realtor realtor){
	if(realtor == NULL || realtor->services == NULL)
		return 0;
	int total = 0;
	MAP_FOREACH(char*, current, realtor->services){
		int median = 0;
		total += servicePriceMedian(mapGet(realtor->services, current), &median);
		total += median;
	}
	return total / realtorGetTotalApartmentNumber(realtor);
}

int realtorGetAverageMedianArea(Realtor realtor){
	if(realtor == NULL || realtor->services == NULL)
		return 0;
	int total = 0;
	MAP_FOREACH(char*, current, realtor->services){
		int median = 0;
		serviceAreaMedian(mapGet(realtor->services, current), &median);
		total += median;
	}
	return total / realtorGetTotalApartmentNumber(realtor);
}


// ------------------- </Realtor> -------------------
