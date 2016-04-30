/*
 * User.h
 *
 *  Created on: 30 באפר׳ 2016
 *      Author: Liron
 */

#ifndef USER_H_
#define USER_H_


typedef void* userElement;


typedef enum userType_t{
	realtor, customer,
} userType;


typedef struct user_t* User;



// Realtor
User userRealtorCreate(char* company_name, int tax_percentage);

MTMResult userRealtorDestroy(User user);

MTMResult userRealtorAddService(User user, char* service_name, int max_apartments);

MTMResult userRealtorRemoveService(User user, char* service_name);

MTMResult userRealtorAddApartment(User user, char* service_name,
		int id, int price, int width, int height, char* matrix);

MTMResult userRealtorRemoveApartment(User user, char* service_name, int id);

MTMResult userRealtorRespondToOffer(User userRealtor,
		User userCustomer, char* customer_email, Choice choice);

// Customer
User userCustomerCreate(int min_area, int min_rooms, int max_price);

MTMResult userCustomerDestroy(User user);

MTMResult userCustomerPurchase(User userCustomer, User userRealtor,
		char* service_name, int apartment_id);

MTMResult userCustomerMakeOffer(User userCustomer, User userRealtor, int id, int price,
		char* service_name, char* customer_email);



#endif /* USER_H_ */
