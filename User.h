/*
 * User.h
 *
 *  Created on: 30 באפר׳ 2016
 *      Author: Liron
 */


#ifndef USER_H_
#define USER_H_

#include "yad3service.h"
#include "Realtor.h"
#include "Customer.h"

typedef void* userElement;


typedef enum userType_t{
	realtor, customer,
} userType;


typedef struct user_t* User;



// Realtor
User userRealtorCreate(char* company_name, int tax_percentage);

yad3Result userRealtorDestroy(User user);

yad3Result userRealtorAddService(User user, char* service_name, int max_apartments);

yad3Result userRealtorRemoveService(User user, char* service_name);

yad3Result userRealtorAddApartment(User user, char* service_name,
		int id, int price, int width, int height, char* matrix);

yad3Result userRealtorRemoveApartment(User user, char* service_name, int id);

yad3Result userRealtorRespondToOffer(User userRealtor,
		User userCustomer, char* customer_email, Choice choice);

bool userIsRealtorRelevant(User user, Customer target);


// Customer
User userCustomerCreate(int min_area, int min_rooms, int max_price);

yad3Result userCustomerDestroy(User user);

yad3Result userCustomerPurchase(User userCustomer, User userRealtor,
		char* service_name, int apartment_id);

yad3Result userCustomerMakeOffer(User userCustomer, User userRealtor, int id, int price,
		char* service_name, char* customer_email);

userType userGetType(User user);

userElement userGetData(User user);

#endif /* USER_H_ */
