/*
 * Interpreter.c
 *
 *  Created on: 6 במאי 2016
 *      Author: Liron
 */

#include "mtm_ex2.h"
#include "list.h"
#include "yad3service.h"

#include <string.h>


static void strfree(char* str){
	free(str);
}


static MtmErrorCode resultYad3ToMTM(yad3Result result){
	switch(result){
	case YAD3_OUT_OF_MEMORY:
		return MTM_OUT_OF_MEMORY;
		break;
	case YAD3_INVALID_PARAMETERS:
		return MTM_INVALID_PARAMETERS;
		break;
	case YAD3_EMAIL_ALREADY_EXISTS:
		return MTM_EMAIL_ALREADY_EXISTS;
		break;
	case YAD3_EMAIL_DOES_NOT_EXIST:
		return MTM_EMAIL_DOES_NOT_EXIST;
		break;
	case YAD3_EMAIL_WRONG_ACCOUNT_TYPE:
		return MTM_EMAIL_WRONG_ACCOUNT_TYPE;
		break;
	case YAD3_ALREADY_REQUESTED:
		return MTM_ALREADY_REQUESTED;
		break;
	case YAD3_NOT_REQUESTED:
		return MTM_NOT_REQUESTED;
		break;
	case YAD3_APARTMENT_SERVICE_ALREADY_EXISTS:
		return MTM_APARTMENT_SERVICE_ALREADY_EXISTS;
		break;
	case YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST:
		return MTM_APARTMENT_SERVICE_DOES_NOT_EXIST;
		break;
	case YAD3_APARTMENT_SERVICE_FULL:
		return MTM_APARTMENT_SERVICE_FULL;
		break;
	case YAD3_APARTMENT_ALREADY_EXISTS:
		return MTM_APARTMENT_ALREADY_EXISTS;
		break;
	case YAD3_APARTMENT_DOES_NOT_EXIST:
		return MTM_APARTMENT_DOES_NOT_EXIST;
		break;
	case YAD3_PURCHASE_WRONG_PROPERTIES:
		return MTM_PURCHASE_WRONG_PROPERTIES;
		break;
	case YAD3_REQUEST_WRONG_PROPERTIES:
		return MTM_REQUEST_WRONG_PROPERTIES;
		break;
	case YAD3_REQUEST_ILLOGICAL_PRICE:
		return MTM_REQUEST_ILLOGICAL_PRICE;
		break;
	default: return MTM_INVALID_COMMAND_LINE_PARAMETERS;
	// ^^^ this is equivelent to success since there's no MTM_SUCCESS
	}
}

// return value is true if and only if we should close the program
bool printErrorCode(yad3Result result){
	if(result == YAD3_SUCCESS)
		return false;
	MtmErrorCode mtmResult = resultYad3ToMTM(result);
	if(mtmResult == MTM_OUT_OF_MEMORY)
		return true;
	if(mtmResult != MTM_INVALID_COMMAND_LINE_PARAMETERS)
		mtmPrintErrorMessage(stderr, mtmResult);
	return false;
}

/*
 * getInput: turns a line of input into a command, subcommand and list of
 * parameters, and sends them to be interpreted
 * if the interpreting function has found that the program needs to be stopped
 * returns a "true" value
 *
 * @param line - The line of input given to the interpreter.
 */

static bool interpretRealtorInput(yad3service yad3, char* subcommand,
		List arguments, FILE* output){
	char* email = listGetFirst(arguments);
	if(strcmp(subcommand, "add") == 0){
		char* company = (char*)listGetNext(arguments);
		int tax = atoi((char*)listGetNext(arguments));
		return printErrorCode(yad3serviceAddRealtor(yad3, email, company, tax));
	}
	if(strcmp(subcommand, "remove") == 0){
		return printErrorCode(yad3serviceRemoveRealtor(yad3, email));
	}
	else if(strcmp(subcommand, "add_apartment_service") == 0){
		char* service = (char*)listGetNext(arguments);
		int max = atoi((char*)listGetNext(arguments));
		return printErrorCode(yad3serviceRealtorAddApartmentService
				(yad3, email, service, max));
	}
	else if(strcmp(subcommand, "remove_apartment_service") == 0){
		char* service = (char*)listGetNext(arguments);
		return printErrorCode(yad3serviceRealterRemoveApartmentService
				(yad3, email, service));
	}
	else if(strcmp(subcommand, "add_apartment") == 0){
	char* service = (char*)listGetNext(listGetNext(arguments));
		int id = atoi((char*)listGetNext(listGetNext(arguments))),
				price = atoi((char*)listGetNext(listGetNext(arguments))),
				width = atoi((char*)listGetNext(listGetNext(arguments))),
				height = atoi((char*)listGetNext(listGetNext(arguments)));
		char* matrix = (char*)listGetNext(listGetNext(arguments));
		return printErrorCode(yad3serviceRealtorAddApartment(yad3, email,
				service, id, price, width, height, matrix));
	}
	else if(strcmp(subcommand, "remove_apartment") == 0){
		char* service = (char*)listGetNext(listGetNext(arguments));
		int id = atoi((char*)listGetNext(listGetNext(arguments)));
		return printErrorCode(yad3serviceRealtorRemoveApartment(
				yad3, email, service, id));
	}
	else if(strcmp(subcommand, "respond_to_offer") == 0){
		char *customer_email = (char*)listGetNext(arguments),
		*str_choice = (char*)listGetNext(arguments);
		Choice choice;
		if(strcmp(str_choice, "accept") == 0)
			choice = accept;
		else
			choice = decline;
		return printErrorCode(yad3serviceRealtorRespondToOffer(yad3,
				email, customer_email, choice));
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}

static bool interpretCustomerInput(yad3service yad3, char* subcommand,
		List arguments, FILE* output){
	char* email = listGetFirst(arguments);
	if(strcmp(subcommand, "add") == 0){
		int area = atoi(listGetNext(arguments)),
				rooms = atoi((char*)listGetNext(arguments)),
				price = atoi((char*)listGetNext(arguments));
		return printErrorCode(yad3serviceAddCustomer(yad3, email,
				area, rooms, price));
	}
	else if(strcmp(subcommand, "remove") == 0){
		return printErrorCode(yad3serviceRemoveRealtor(yad3, email));
	}
	else if(strcmp(subcommand, "purchase") == 0){
		char* realtor_email = (char*)listGetNext(arguments);
		char* service = (char*)listGetNext(arguments);
		int id = atoi((char*)listGetNext(listGetNext(arguments)));
		return printErrorCode(yad3serviceCustomerPurchase
				(yad3, email, realtor_email, service, id));
	}
	else if(strcmp(subcommand, "make_offer") == 0){
		char* realtor_email = (char*)listGetNext(arguments);
		char* service = (char*)listGetNext(arguments);
		int id = atoi((char*)listGetNext(arguments)),
				new_price = atoi((char*)listGetNext(arguments));
		return printErrorCode(yad3serviceCustomerMakeOffer(yad3, email,
				realtor_email, service, id, new_price));
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}

static bool interpretReportInput(yad3service yad3, char* subcommand,
		List arguments, FILE* output){
	if(strcmp(subcommand, "relevant_realtors") == 0){
		char* email = (char*)listGetFirst(arguments);
		return printErrorCode(yad3serviceReportRelevantRealtors(yad3, email));
	}
	else if(strcmp(subcommand, "most_paying_customers") == 0){
		int count = atoi((char*)listGetFirst(arguments));
		return printErrorCode(yad3serviceReportMostPayingCustomers
				(yad3, count));
	}
	else if(strcmp(subcommand, "significant_realtors") == 0){
		int count = atoi((char*)listGetFirst(arguments));
		return printErrorCode(yad3serviceReportSignificantRealtors
				(yad3, count));
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}

static bool interpretInput(yad3service yad3,
		char* command, char* subcommand, List arguments, FILE* output){
	if(strcmp(command, "realtor") == 0){
		return interpretRealtorInput(yad3, subcommand, arguments, output);
	}
	else if(strcmp(command, "customer") == 0){
		return interpretCustomerInput(yad3, subcommand, arguments, output);
	}
	else if(strcmp(command, "report") == 0){
		interpretReportInput(yad3, subcommand, arguments, output);
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}

static bool runLine(yad3service yad3, char* line, FILE* output){
	char *word, *command, *subcommand;
	List arguments = listCreate((CopyListElement)strcpy,
			(FreeListElement)strfree);
	int count = 0;
	do{
		word = strtok(line, " ");
		if(word == NULL)
			break;
		if(count == 0){
			if(strchr(word, '#') != NULL)	// if the word is a comment
				break;
			command = word;
		}
		else if(count == 1)
			subcommand = word;
		else
			listInsertLast(arguments, word);
	}
	while(word != NULL);
	bool stop_running = interpretInput(yad3, command, subcommand, arguments, output);
	return stop_running;
}

static void fileOpenedProperly(FILE* file, bool* close_program){
	if(file == NULL){
		*close_program = true;
	}
}


int main1(int argc, char** argv){
	// <<<< check when to use fopen
	bool close_program;
	FILE *input = stdin, *output = stdout; // search standard input
	if(argc == 0){	// if input and output aren't given
	}
	else if(argc == 2){	 // if only input or only output are given
		if(strcmp(argv[0],"i") == 0){
			input = fopen(argv[1], "r");
			fileOpenedProperly(input, &close_program);
		}
		else if(strcmp(argv[0],"o") == 0){
			output = fopen(argv[1], "w");
			fileOpenedProperly(output, &close_program);
		}
		else
			mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	}
	else if(argc == 4){	// if input and output are given
		if(strcmp(argv[0],"i") == 0){
			input = fopen(argv[1], "r");
			fileOpenedProperly(input, &close_program);
		}
		else if(strcmp(argv[0],"o") == 0){
			output = fopen(argv[1], "w");
			fileOpenedProperly(output, &close_program);
		}
		else
			mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		if(strcmp(argv[2],"i") == 0){
			input = fopen(argv[3], "r");
			fileOpenedProperly(input, &close_program);
		}
		else if(strcmp(argv[2],"o") == 0){
			output = fopen(argv[3], "w");
			fileOpenedProperly(output, &close_program);
		}
		else
			mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	}
	else
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);

	char* line = "";	//  necessary only to avoid the uninitialized warning
	yad3service yad3 = yad3serviceCreate();
	while(fgets(line, MAX_LEN, input) && !close_program){
		close_program = runLine(yad3, line, output);
	}
	yad3serviceDestroy(yad3);
}



