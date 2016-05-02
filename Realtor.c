/*
 * Realtor.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#include "Realtor.h"

// ------------------- <General Static functions> -------------------

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
	realtor->company_name = str_copy(company_name, malloc(sizeof(char)));// this might fail badly
	realtor->tax_percentage = tax_percentage;	// please check ^^^^
	/* I HAVE NO IDEA HOW TO CREATE THESE MAPS
	realtor->services = mapCreate();
	realtor->offers = mapCreate();
	*/
	return realtor;
}

void realtorDestroy(Realtor realtor){
	mapDestroy(realtor->services);
	free(realtor->company_name);
	free(realtor);
}



yad3Result realtorAddService(Realtor realtor, char* service_name, int max_apartments){
	if(serviceExists(realtor, service_name))
		return YAD3_APARTMENT_SERVICE_ALREADY_EXISTS;	// WHY IS THIS NOT RECOGNISED???
	ApartmentService newService = serviceCreate(max_apartments);
	mapPut(realtor->services, service_name, newService);
	return YAD3_SUCCESS;
}

yad3Result realtorRemoveService(Realtor realtor, char* service_name){
	if(!serviceExists(realtor, service_name))
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST;	// WHY IS THIS NOT RECOGNISED???
	mapRemove(realtor->services, service_name);
	return YAD3_SUCCESS;
}

yad3Result realtorAddApartment(Realtor realtor, char* service_name,
		int id, int price, int width, int height, char* matrix){
	if(!serviceExists(realtor, service_name))
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST;
	SquareType** squares = translateMatrix(width, height, matrix);
	if(squares == NULL)
		return YAD3_OUT_OF_MEMORY;	// WHY IS THIS NOT RECOGNISED???
	Apartment apartment = apartmentCreate(squares, height, width, price);
	if(apartment == NULL)
		return YAD3_OUT_OF_MEMORY;	// WHY IS THIS NOT RECOGNISED???
	ApartmentServiceResult result =
			serviceAddApartment(mapGet(realtor->services, service_name), apartment, id);
	if(result == APARTMENT_SERVICE_FULL)
		return YAD3_APARTMENT_SERVICE_FULL;	// WHY IS THIS NOT RECOGNISED???
	if(result == APARTMENT_SERVICE_ALREADY_EXISTS)
		return YAD3_APARTMENT_ALREADY_EXISTS;	// WHY IS THIS NOT RECOGNISED???
	return YAD3_SUCCESS;
}

yad3Result realtorRemoveApartment(Realtor realtor, char* service_name, int id){
	if(!serviceExists(realtor, service_name))
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST; // WHY IS THIS NOT RECOGNISED???
	ApartmentServiceResult result =
			serviceDeleteById(mapGet(realtor->services, service_name), id);
	if(result == APARTMENT_SERVICE_NO_FIT)
		return YAD3_APARTMENT_DOES_NOT_EXIST; // WHY IS THIS NOT RECOGNISED???
	return APARTMENT_SUCCESS;
}
// needs to make the purchase itself
yad3Result realtorRespondToOffer(Realtor realtor, char* customer_email, Choice choice){
	if(!mapContains(realtor->offers, customer_email))
		return YAD3_NOT_REQUESTED;	// WHY IS THIS NOT RECOGNISED???
	if(choice == accept){
		Offer offer = mapGet(realtor->offers, customer_email);
		realtorRemoveApartment(realtor, offerGetServiceName(offer),
				offerGetId(offer));
	}
	mapRemove(realtor->offers, customer_email);
	return YAD3_SUCCESS;
}

bool realtorIsRealtorRelevant(Realtor realtor, Customer customer){
	if(realtor == NULL || customer == NULL)
		return YAD3_NULL_ARGUMENT;
	for(char* iterator = mapGetFirst(realtor->services);
			iterator != NULL; iterator = mapGetNext(realtor->services)){
		if(serviceSearch(mapGet(realtor->services, iterator),
				customerGetMinArea(customer), customerGetMinRooms(customer),
				customerGetMaxPrice(customer), NULL)
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


// ------------------- </Realtor> -------------------
