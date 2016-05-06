/*
 * main.c
 *
 *  Created on: 6 במאי 2016
 *      Author: Liron
 */
#include "customer.h"


#define TEST_EQUALS(res_arg, a,b) 	if (res_arg && (a != b)) { \
										res_arg = false; \
									}



#define RUN_TEST(name)  printf("Running "); \
						printf(#name);		\
						printf("... ");		\
						if (!name()) { \
							printf("[FAILED]\n");		\
							return false; \
						}								\
						printf("[SUCCESS]\n");

bool customerTest(){
	// -------------<customerCreate>-------------
	bool final = true;
	TEST_EQUALS(final, customerCreate(0, 50, 100000), NULL);
	TEST_EQUALS(final, customerCreate(50, 0, 100000), NULL);
	TEST_EQUALS(final, customerCreate(50, 4, 0), NULL);
	Customer customer = customerCreate(50, 4, 1000000);
	// -------------</customerCreate>-------------


	// -------------<customerCopy>-------------
	Customer copy = customerCopy(customer);
	// -------------</customerCopy>-------------
	customerDestroy(customer);
	customerDestroy(copy);

	return final;
}

int main(){
	RUN_TEST(customerTest);
}
