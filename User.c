/*
 * User.c
 *
 *  Created on: 28 באפר׳ 2016
 *      Author: Liron
 */

// for Ziv :D

#include "User.h"


// ------------------- <General Static functions> -------------------


// ------------------- </General Static functions> -------------------

// ------------------- <User> -------------------

/*
 * here's how we built this struct
 * because a user can either be a realtor or a customer, we
 * User will have a "type" field, which indicates where the user
 * is a customer or a realtor
 * and a "data" field, which is the pointer to the user itself
 * using type we know how we should interpret the data, and only
 * need to run the corresponding functions in each struct
 *
 */

// Definitions and general purpose functions  -----------------------

struct user_t {
	userType type;	// defined in yad3service.h, maybe we should move this here
	userElement data;
};

// the user of yad3 isn't meant to use these functions directly
static User userCreate(userType type, userElement data)
{
	User user = malloc(sizeof(*user));
	if(user == NULL){
		return NULL;
	}
	user->type = type;
	user->data = data;	// data is a pointer, so this copies the pointer itself
	return user;
}

static void userDestroy(User user)
{
	if(user->type == customer){
		customerDestroy((Customer*)(user->data));
	}
	else if(user->type == realtor){
		realtorDestroy((Realtor*)(user->data));
	}

}

// Realtor functions ------------------------------------------------------


User userRealtorCreate(char* company_name, int tax_percentage){
	Realtor user = realtorCreate(company_name, tax_percentage);	// don't name this
	if(user == NULL)	// "realtor" because it collides with the enum realtor
		return NULL;
	return userCreate(realtor, user);
}

yad3Result userRealtorDestroy(User user){
	if(user->type != realtor)
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	userDestroy(user);
	return YAD3_SUCCESS;
}

yad3Result userRealtorAddService(User user, char* service_name, int max_apartments){
	if(user->type == realtor){
		return realtorAddService((Realtor*)(user->data), service_name, max_apartments);
	}
	else
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
}

yad3Result userRealtorRemoveService(User user, char* service_name){
	if(user->type == realtor){
		return realtorRemoveService((Realtor*)(user->data), service_name);
	}
	else
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
}

yad3Result userRealtorAddApartment(User user, char* service_name,
		int id, int price, int width, int height, char* matrix){
	if(user->type == realtor){
		return realtorAddApartment((Realtor*)(user->data), service_name,
				id, price, width, height, matrix);
	}
	else
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
}

yad3Result userRealtorRemoveApartment(User user, char* service_name, int id){
	if(user->type == realtor){
		return realtorRemoveApartment((Realtor*)(user->data), service_name, id);
	}
	else
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
}


yad3Result userRealtorRespondToOffer(User userRealtor,
		User userCustomer, char* customer_email, Choice choice){
	if(userCustomer->type != customer || userRealtor->type != realtor)
			return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	return realtorRespondToOffer(userRealtor, customer_email, choice);
}

 bool userIsRealtorRelevant(User user, Customer target){
	 if(user->type != realtor)
		 return false;
	 return realtorIsRealtorRelevant((User)(user->data), target); // <<< probably wrong casting
 }


// Customer functions ------------------------------------------------------

User userCustomerCreate(int min_area, int min_rooms, int max_price){
	Customer user = customerCreate(min_area, min_rooms, max_price);
	if(user == NULL)
		return NULL;
	return userCreate(customer, user);
}

yad3Result userCustomerDestroy(User user){
	if(user->type != customer)
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	userDestroy(user);
	return YAD3_SUCCESS;
}

yad3Result userCustomerPurchase(User userCustomer, User userRealtor,
		char* service_name, int apartment_id){
	if(userCustomer->type != customer || userRealtor->type != realtor)
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	return customerPurchase((Customer*)(userCustomer->data),
			(Realtor*)(userRealtor->data), service_name, apartment_id);
}

yad3Result userCustomerMakeOffer(User userCustomer, User userRealtor, int id, int price,
		char* service_name, char* customer_email){
	if(userCustomer->type != customer || userRealtor->type != realtor)
		return YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
	return customerMakeOffer((Customer*)(userCustomer->data), (Realtor*)(userRealtor->data),
			id, price, service_name, customer_email);
}

userType userGetType(User user){
	return user->type;
}

userElement userGetData(User user){
	return user->data;
}



// ------------------- </User> -------------------
