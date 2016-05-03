/*
 * Customer.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_


#include "Realtor.h"	// this is valid, customer knows realtor but no vice versa

typedef struct Customer_t* Customer;

typedef enum customerResult_t {
	CUSTOMER_PURCHASE_WRONG_PROPERTIES,	// Customer errors
	CUSTOMER_ALREADY_REQUESTED,
	CUSTOMER_REQUEST_WRONG_PROPERTIES,
	CUSTOMER_REQUEST_ILLOGICAL_PRICE,
	CUSTOMER_NOT_REQUESTED,
	CUSTOMER_SUCCESS,
	CUSTOMER_OUT_OF_MEMORY,
	CUSTOMER_NULL_ARGUMENT,
	CUSTOMER_REQUEST_WRONG_PPROPERTIES,
	CUSTOMER_APARTMENT_DOES_NOT_EXIST,
	CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST
} customerResult;

Customer customerCreate(int min_area, int min_rooms, int max_price);

Customer customerCopy(Customer customer);

void customerDestroy(Customer customer);

customerResult customerPurchase(Customer customer,
	Realtor realtor, char* service_name, int apartment_id);

customerResult customerMakeOffer(Customer customer, Realtor realtor, int id,
		int price, char* service_name, char* customer_email);

int customerGetMinArea(Customer customer);

int customerGetMinRooms(Customer customer);

int customerGetMaxPrice(Customer customer);

int customerGetTotalPayment(Customer customer);

#endif /* CUSTOMER_H_ */
