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

// not static, but still useful both in yad3service and customer
bool apartmentCorrectProperties(Customer customer, Apartment apartment){
	return apartmentGetPrice(apartment) <= customer->max_price
			&& apartmentTotalArea(apartment) <= customer->min_area
			&& apartmentNumOfRooms(apartment) <= customer->min_rooms;
}
// ------------------- </General Static functions> -------------------


