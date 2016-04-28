/*
 * Realtor.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

#include "yad3service.c"
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
typedef struct Realtor_t* Realtor;

struct Realtor_t{
	Map services;	// keys are service names, data is services
	char* company_name;
	int tax_percentage;
	Map offers;		// keys are customer emails, data is offers
};

static bool serviceExists(Realtor realtor, char* service_name){
	return mapContains(realtor->services, service_name);
}

static MTMResult addOffer(Realtor realtor, int id, int price,
		char* service_name, char* customer_email){
	if(mapContains(realtor->offers, customer_email))
		return MTM_ALREADY_REQUESTED;
	if(!serviceExists(realtor, service_name))
		return MTM_APARTMENT_SERVICE_DOES_NOT_EXIST;
	Apartment apartment;
	serviceGetById((realtor->services, service_name), id, apartment);
	if(apartment == NULL)
		return MTM_APARTMENT_DOES_NOT_EXIST;
	// ------------------------------------ check if apartment fits customer's needs

			// somewhere here we return MTM_REQUEST_WRONG_PPROPERTIES

	// ends of customer checks

	if(apartmentGetPrice(apartment) > price)	// not sure if I got the instruction right
		return MTM_REQUEST_ILLOGICAL_PRICE;		// <<<< about this error
	Offer offer = offerCreate(id, price, service_name);
	mapPut(realtor->offers, customer_email, offer);
	apartmentDestroy(apartment); // apartment is a copy and needs to be dealt with
	return MTM_SUCCESS;
}

// Functions -------------------

Realtor realtorCreate(char* company_name, char* tax_percentage){
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

MTMResult realtorAddService(Realtor realtor, char* service_name, int max_apartments){
	if(serviceExists(realtor, service_name))
		return MTM_APARTMENT_SERVICE_ALREADY_EXISTS;	// WHY IS THIS NOT RECOGNISED???
	ApartmentService newService = serviceCreate(max_apartments);
	mapPut(realtor->services, service_name, newService);
	return MTM_SUCCESS;
}

MTMResult realtorRemoveService(Realtor realtor, char* service_name){
	if(!serviceExists(realtor, service_name))
		return MTM_APARTMENT_SERVICE_DOES_NOT_EXIST;	// WHY IS THIS NOT RECOGNISED???
	mapRemove(realtor->services, service_name);
	return MTM_SUCCESS;
}

MTMResult realtorAddApartment(Realtor realtor, char* service_name,
		int id, int price, int width, int height, char* matrix){
	if(!serviceExists(realtor, service_name))
		return MTM_APARTMENT_SERVICE_DOES_NOT_EXIST;
	SquareType** squares = translateMatrix(width, height, matrix);
	if(squares == NULL)
		return MTM_OUT_OF_MEMORY;	// WHY IS THIS NOT RECOGNISED???
	Apartment apartment = apartmentCreate(squares, height, width, price);
	if(apartment == NULL)
		return MTM_OUT_OF_MEMORY;	// WHY IS THIS NOT RECOGNISED???
	ApartmentServiceResult result =
			serviceAddApartment(mapGet(realtor->services, service_name), apartment, id);
	if(result == APARTMENT_SERVICE_FULL)
		return MTM_APARTMENT_SERVICE_FULL;	// WHY IS THIS NOT RECOGNISED???
	if(result == APARTMENT_SERVICE_ALREADY_EXISTS)
		return MTM_APARTMENT_ALREADY_EXISTS;	// WHY IS THIS NOT RECOGNISED???
	return MTM_SUCCESS;
}

MTMResult realtorRemoveApartment(Realtor realtor, char* service_name, int id){
	if(!serviceExists(realtor, service_name))
		return MTM_APARTMENT_SERVICE_DOES_NOT_EXIST; // WHY IS THIS NOT RECOGNISED???
	ApartmentServiceResult result =
			serviceDeleteById(mapGet(realtor->services, service_name), id);
	if(result == APARTMENT_SERVICE_NO_FIT)
		return MTM_APARTMENT_DOES_NOT_EXIST; // WHY IS THIS NOT RECOGNISED???
	return APARTMENT_SUCCESS;
}

MTMResult realtorRespondToOffer(Realtor realtor, char* customer_email, Choice choice){
	if(!mapContains(realtor->offers, customer_email))
		return MTM_NOT_REQUESTED;	// WHY IS THIS NOT RECOGNISED???
	if(choice == accept){
		Offer offer = mapGet(realtor->offers, customer_email);
		realtorRemoveApartment(realtor, offer->service_name, offer->id);
	}
	mapRemove(realtor->offers, customer_email);
}

// ------------------- </Realtor> -------------------
