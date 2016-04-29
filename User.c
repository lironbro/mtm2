/*
 * User.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

// for Ziv :D

#include "yad3service.h"

typedef void* userElement;
// ^should be written in user.h and included here.

typedef struct user_t* User;

struct user_t {
	char* type;
	userElement address;
	userElement realUser;
};

User createUser(char* type, userElement address, userElement realUser)
{
	User user = malloc(sizeof(*user));
	if(user == NULL){
		return NULL;
	}
	user->type=type;
	user->address=address;
	user->realUser=realUser;
	return user;
}

void userDestroy(User user)
{
	if(user->type == "Customer"){
		customerDestroy((Customer*)(user->realUser));
	}
	else if(user->type == "Realtor"){
		realtorDestroy((Realtor*)(user->realUser));
	}

}
//DOES NOT RETURN THE INTERNAL ERRORS. Not sure how to return them.
MTMResult offer(User customer, User realtor, int id, int price,
		char* service_name, char* customer_email, Choice choice)
{
	if(customer->realUser!="customer" || realtor->realUser!="Realtor"){
		return NULL;
	}
	if(customerMakeOffer(customer, realtor, id, price,
			service_name, customer_email) == MTM_SUCCESS){
		if(realtorRespondToOffer(realtor, customer_email, choice) == MTM_SUCCESS){
			return customerPurchase(customer, realtor, service_name, id); //Not sure if id is the apartment's ID.
		}
	}
	return NULL;
}

MTMResult userAddService(User user, char* service_name, int max_apartments)
{
	if(user->type=="Realtor"){
		return realtorAddService((Realtor*)(user->realUser), service_name, max_apartments);
	}
	else
		return NULL;
}

MTMResult userRemoveService(User user, char* service_name)
{
	if(user->type=="Realtor"){
		return realtorRemoveService((Realtor*)(user->realUser), service_name);
	}
	else
		return NULL;
}

MTMResult userAddApartment(User user, char* service_name,
		int id, int price, int width, int height, char* matrix)
{
	if(user->type=="Realtor"){
		return realtorAddApartment((Realtor*)(user->realUser), service_name,
				id, price, width, height, matrix);
	}
	else
		return NULL;
}

MTMResult userRemoveApartment(User user, char* service_name, int id)
{
	if(user->type=="Realtor"){
		return realtorRemoveApartment((Realtor*)(user->realUser), service_name, id);
	}
	else
		return NULL;
}
