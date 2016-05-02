/*
 * Customer.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "yad3service.h"
#include "Realtor.h"	// <<<< might be because of double including


typedef struct Customer_t* Customer;


Customer customerCreate(int min_area, int min_rooms, int max_price);

void customerDestroy(Customer customer);

yad3Result customerPurchase(Customer customer,
	Realtor realtor, char* service_name, int apartment_id);

yad3Result customerMakeOffer(Customer customer, Realtor realtor, int id,
		int price, char* service_name, char* customer_email);

int customerGetMinArea(Customer customer);

int customerGetMinRooms(Customer customer);

int customerGetMaxPrice(Customer customer);

#endif /* CUSTOMER_H_ */
