/*
 * Customer.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */


#include "Customer.h"


// ------------------- <General Static functions> -------------------


/*
 * checkValidProperties: checks that the given properties fit the customer
 *
 * @param customer_max_price: max price for customer
 * @param customer_min_area: min area for customer
 * @param customer_min_rooms: min rooms for customer
 * @param price: the price being checked
 * @param rooms: the rooms being checked
 * @param area: the area being checked
 */
static bool checkValidProperties(int customer_max_price, int customer_min_area,
		int customer_min_rooms, int price, int rooms, int area){
	return area >= customer_min_area &&
			price <= customer_max_price &&
			rooms >= customer_min_rooms;
}

/*
 * apartmentCorrectProperties: checks that an apartment has correct properties
 * 							   for a given customer
 *
 * @param customer: the customer for whom the apartment is being bought
 * @param apartment: the apartment being bought
 *
 * returns:
 * true if apartment fits the customer
 * false otherwise
 */
static bool apartmentCorrectProperties(Customer customer, Apartment apartment){
	if(apartment == NULL || customer == NULL)
		return false;
	return checkValidProperties(customerGetMaxPrice(customer),
			customerGetMinArea(customer), customerGetMinRooms(customer),
			apartmentGetPrice(apartment), apartmentNumOfRooms(apartment),
			apartmentTotalArea(apartment));
}

/*
 * offerCorrectProperties: checks that an offer has correct properties.
 *
 * @param customer: the customer for whom the apartment is being offered
 * @param apartment: the apartment being offered
 * @param new_price: the price being set for the apartment in the offer
 *
 * returns:
 * true if apartment fits the customer
 * false otherwise
 */
static bool offerCorrectProperties(Customer customer, Apartment apartment,
		int new_price){
	if(customer == NULL || apartment == NULL)
		return false;
	return checkValidProperties(customerGetMaxPrice(customer),
			customerGetMinArea(customer), customerGetMinRooms(customer),
			new_price, apartmentNumOfRooms(apartment),
			apartmentTotalArea(apartment));
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
	if(min_area <= 0 || min_rooms <= 0 || max_price <= 0)
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
	Apartment apartment = NULL;
	if(service == NULL)
		return CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST;
	serviceGetById(service, apartment_id, &apartment);	// creates a >>COPY<<
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
	serviceDeleteById(service, apartment_id); // removes apartment from service
	return CUSTOMER_SUCCESS;
}


customerResult customerMakeOffer(Customer customer, Realtor realtor,
		char* service_name, char* customer_email, int id, int price){
	if(mapContains(realtorGetOffers(realtor), customer_email))
		return CUSTOMER_ALREADY_REQUESTED;
	if(!mapContains(realtorGetServices(realtor), service_name))
		return CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST;
	Apartment apartment;
	serviceGetById(mapGet(realtorGetServices(realtor), service_name),
			id, &apartment);
	if(apartment == NULL)
		return CUSTOMER_APARTMENT_DOES_NOT_EXIST;
	if(!offerCorrectProperties(customer, apartment, price)){
		apartmentDestroy(apartment);
		return CUSTOMER_REQUEST_WRONG_PROPERTIES;
	}
	if(apartmentGetPrice(apartment) < price){
		apartmentDestroy(apartment);
		return CUSTOMER_REQUEST_ILLOGICAL_PRICE;
	}
	Offer offer = offerCreate(id, price, service_name);
	mapPut(realtorGetOffers(realtor), customer_email, offer);
	offerDestroy(offer);
	customer->total_payment += price;
	apartmentDestroy(apartment); // apartment is a copy and needs to be erased
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

