/*
 * Customer.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Liron
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_


#include "Realtor.h"	// customer knows realtor but no vice versa

typedef struct Customer_t* Customer;

typedef enum customerResult_t {
	CUSTOMER_INVALID_PARAMETERS,
	CUSTOMER_PURCHASE_WRONG_PROPERTIES,
	CUSTOMER_ALREADY_REQUESTED,
	CUSTOMER_REQUEST_WRONG_PROPERTIES,
	CUSTOMER_REQUEST_ILLOGICAL_PRICE,
	CUSTOMER_NOT_REQUESTED,
	CUSTOMER_SUCCESS,
	CUSTOMER_OUT_OF_MEMORY,
	CUSTOMER_NULL_ARGUMENT,
	CUSTOMER_APARTMENT_DOES_NOT_EXIST,
	CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST
} customerResult;

/*
 * customerCreate: creates a new customer
 *
 * @param min_area: the minimum area needed for the customer to purchase or
 * 					make an offer for an apartment
 * @param min_rooms: the minimum number of rooms needed for the customer to
 * 					 purchase or make an offer for an apartment
 * @param max_price: the maximum price needed for the customer to purchase or
 * 					make an offer for an apartment
 */
Customer customerCreate(int min_area, int min_rooms, int max_price);

/*
 * customerCopy: copies a customer
 *
 * @param customer: the customer to copy
 */
Customer customerCopy(Customer customer);

/*
 * customerDestroy: destroys a customer
 *
 * @param customer: the customer in need of destroying
 */
void customerDestroy(Customer customer);

/*
 * customerPurchase: purchases an apartment, meaning it adds its price, with
 * 					 tax, to the customer's total payment, and removes it
 * 					 from the realtor's service
 * @param customer: the customer making the purchase
 * @param realtor: the realtor from whom the purchase is being made
 * @param service_name: the service in which the apartment is locaed
 * @param id: the id of the apartment in the service
 *
 * returns:
 * CUSTOMER_SUCCESS if the purchase has been made
 * CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no apartment service
 * 					with the given name in the realtor's possession
 * CUSTOMER_APARTMENT_DOES_NOT_EXIST if there is no apartment with the given
 * 					id in the service's array
 * CUSTOMER_PURCHASE_WRONG_PROPERTIES if the apartment does not answer the
 * 					customer's demands
 */
customerResult customerPurchase(Customer customer,
	Realtor realtor, char* service_name, int apartment_id);

/*
 * customerMakeOffer: sends the realtor an offer from the customer for an
 * 					  apartment with the specified id in the specified service
 *
 * @param customer: the customer sending the offer
 * @param realtor: the realtor to whom the offer is being sent
 * @param service_name: the service in which the desired apartment is located
 * @param customer_email: the email of the customer
 * @param id: the id of the desired apartment
 * @param price: the price being offered for the apartment
 *
 * returns:
 * CUSTOMER_SUCCESS if the offer has been made successfuly
 * CUSTOMER_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no apartment service
 * 					with the given name in the realtor's possession
 * CUSTOMER_APARTMENT_DOES_NOT_EXIST if there is no apartment with the given
 * 					id in the service's array
 * CUSTOMER_REQUEST_WRONG_PROPERTIES if the apartment does not answer the
 * 					customer's demands
 * CUSTOMER_ALREADY_REQUESTED if the customer has already made an offer to
 * 					this realtor
 * CUSTOMER_REQUEST_ILLOGICAL_PRICE if the price being offered for the
 * 					apartment is higher than its worth, but lower
 * 					than the maximum price of the customer
 */
customerResult customerMakeOffer(Customer customer, Realtor realtor,
		char* service_name, char* customer_email, int id, int price);

/*
 * customerGetMinArea: returns the customer's minimum area
 */
int customerGetMinArea(Customer customer);

/*
 * customerGetMinRooms: returns the customer's minimum rooms
 */
int customerGetMinRooms(Customer customer);

/*
 * customerGetMaxPrice: returns the customer's maximum price
 */
int customerGetMaxPrice(Customer customer);

/*
 * customerGetTotalPayment: returns the customer's total payment
 */
int customerGetTotalPayment(Customer customer);

#endif /* CUSTOMER_H_ */
