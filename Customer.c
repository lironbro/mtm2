/*
 * Customer.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */


#include "Customer.h"


// ------------------- <General Static functions> -------------------

static bool apartmentCorrectProperties(Customer customer, Apartment apartment){
	if(apartment == NULL || customer == NULL)
		return false;
	return !(apartmentTotalArea(apartment) < customerGetMinArea(customer) ||
			apartmentGetPrice(apartment) > customerGetMaxPrice(customer) ||
			apartmentNumOfRooms(apartment) < customerGetMinRooms(customer));
}
// ------------------- </General Static functions> -------------------


// ------------------- <Customer> -------------------
/*
 * here's the general structure of this struct
 * it contains the basic traits given to it in the create function
 * maybe some additional traits I'll think of during writing this struct
 * but it does NOT contain its own email or type, as these traits
 * belong to the more generic User struct which I will construct in the future
 * it assumes MOST inputs to be true, unless they can't be tested in the user
 * struct, then it tests them here, atleast it does in theory
 *
 */
struct Customer_t{
	int min_area, min_rooms, max_price;
	int total_payment;
};

Customer customerCreate(int min_area, int min_rooms, int max_price){
	if(min_area < 0 || min_rooms < 0 || max_price < 0)
		return NULL;
	Customer customer = malloc(sizeof(*customer));
	customer->min_area = min_area;
	customer->max_price = max_price;
	customer->min_rooms = min_rooms;
	customer->total_payment = 0;
	return customer;
}

Customer customerCopy(Customer customer){
	Customer copy = customerCreate(customer->min_area, customer->min_rooms,
			customer->max_price);
	copy->total_payment = customer->total_payment;
	return copy;
}

void customerDestroy(Customer customer){
	free(customer);
}



customerResult customerPurchase(Customer customer,
	Realtor realtor, char* service_name, int apartment_id){
	ApartmentService service =
			mapGet(realtorGetServices(realtor), service_name);
	Apartment apartment;
	if(service == NULL)
		return CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST;
	serviceGetById(service, apartment_id, &apartment);
	if(apartment == NULL)
		return CUSTOMER_APARTMENT_DOES_NOT_EXIST;
	if(!apartmentCorrectProperties(customer, apartment)){
		apartmentDestroy(apartment);
		return CUSTOMER_PURCHASE_WRONG_PROPERTIES;
	}
	customer->total_payment +=
			apartmentGetPrice(apartment)*
			(100+realtorGetTaxPercentage(realtor))/100;
	apartmentDestroy(apartment);	// deletes the copy used for the function
	serviceDeleteById(service, apartment_id);	// removes apartment from service
	return CUSTOMER_SUCCESS;
}

// yes, this function is unattractive to say the least, and also has
// an almost duplication of parameters, them being "customer"
// and "customer_email", couldn't figure how to cut out one of them
// so I left it as is
customerResult customerMakeOffer(Customer customer, Realtor realtor, int id, int price,
		char* service_name, char* customer_email){
	if(mapContains(realtorGetOffers(realtor), customer_email))
		return CUSTOMER_ALREADY_REQUESTED;
	if(!mapContains(realtorGetServices(realtor), service_name))
		return CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST;
	Apartment apartment;
	serviceGetById(mapGet(realtorGetServices(realtor), service_name),
			id, &apartment);
	if(apartment == NULL)
		return CUSTOMER_APARTMENT_DOES_NOT_EXIST;
	if(!apartmentCorrectProperties(customer, apartment))
		return CUSTOMER_REQUEST_WRONG_PPROPERTIES;
	if(apartmentGetPrice(apartment) <= price)	// not sure if I got the instruction right
		return CUSTOMER_REQUEST_ILLOGICAL_PRICE;		// <<<< about this error
	Offer offer = offerCreate(id, price, service_name);
	mapPut(realtorGetOffers(realtor), customer_email, offer);
	customer->total_payment += price;
	apartmentDestroy(apartment); // apartment is a copy and needs to be dealt with
	return CUSTOMER_SUCCESS;
}

int customerGetMinArea(Customer customer){
	return customer->min_area;
}

int customerGetMinRooms(Customer customer){
	return customer->min_rooms;
}

int customerGetMaxPrice(Customer customer){
	return customer->max_price;
}

int customerGetTotalPayment(Customer customer){
	return customer->total_payment;
}

