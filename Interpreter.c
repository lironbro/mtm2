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

/*
 * strfree: frees strings
 *
 * @param str- the string to free
 */
static void strfree(char* str){
	free(str);
}

/*
 * resultYad3ToMTM: converts a yad3Result to a MtmErrorCode
 * note that there is no MTM_SUCCESS result, meaning that we turn YAD3_SUCCESS
 * to MTM_INVALID_COMMAND_LINE_PARAMETERS, since this error should be
 * handled earlier by the interpreting functions
 *
 * also: this function is exactly 50 lines, which just about fits the old
 * conventions. splitting this up to fit the new conventions would make
 * this function awful, instead of being this easy to understand
 *
 *
 * @param result - the yad3Result in need of conversion
 *
 * return: the equivalent MtmErrorCode, or MTM_INVALID_COMMAND_LINE_PARAMETERS
 * 		   if result was YAD3_SUCCESS
 */
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
	// ^^^ this is equivalent to success since there's no MTM_SUCCESS
	}
}

// return value is true if and only if we should close the program
/*
 * printErrorCode: turns a yad3Result to a MtmErrorCode and prints it if
 * it's not MTM_INVALID_COMMAND_LINE_PARAMETERS (read the previous function)
 * also, returns true if the error was found to be worth closing
 *
 * @param result- the result which will be converted and printed if found to
 *  			  be worth printing
 *
 *  return true if the error was MTM_OUT_OF_MEMORY, which is the only error
 *  which needs closing that could happen at this point
 */
bool printErrorCode(yad3Result result){
	if(result == YAD3_SUCCESS)
		return false;
	MtmErrorCode mtmResult = resultYad3ToMTM(result);
	if(mtmResult == MTM_OUT_OF_MEMORY)
		return true;
	if(mtmResult != MTM_INVALID_COMMAND_LINE_PARAMETERS)	// unnecessary test
		mtmPrintErrorMessage(stderr, mtmResult);			// but remindful
	return false;
}



// <Realtor commands>

/*
 * interpreterRealtorAdd: assuming the command was relevant to
 * realtor, and the subcommand was to "add", performs the
 *  function and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterRealtorAdd(yad3service yad3, char* email,
		List arguments){
	char* company = (char*)listGetNext(arguments);
	int tax = atoi((char*)listGetNext(arguments));
	return printErrorCode(yad3serviceAddRealtor(yad3, email, company, tax));
}


/*
 * interpreterRealtorAddService: assuming the command was relevant to
 * realtor, and the subcommand was to "add_apartment_service", performs the
 *  function and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterRealtorAddService(yad3service yad3, char* email,
		List arguments){
	char* service = (char*)listGetNext(arguments);
	int max = atoi((char*)listGetNext(arguments));
	return printErrorCode(yad3serviceRealtorAddApartmentService
			(yad3, email, service, max));
}


/*
 * interpreterRealtorRemoveService: assuming the command was relevant to
 * realtor, and the subcommand was to "remove_apartment_service", performs the
 *  function and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterRealtorRemoveService(yad3service yad3, char* email,
		List arguments){
	char* service = (char*)listGetNext(arguments);
	return printErrorCode(yad3serviceRealterRemoveApartmentService
			(yad3, email, service));
}


/*
 * interpreterRealtorAddApartment: assuming the command was relevant to
 * realtor, and the subcommand was to "add_apartment", performs the function
 * and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterRealtorAddApartment(yad3service yad3, char* email,
		List arguments){
	char* service = (char*)listGetNext(listGetNext(arguments));
	int id = atoi((char*)listGetNext(listGetNext(arguments))),
			price = atoi((char*)listGetNext(listGetNext(arguments))),
			width = atoi((char*)listGetNext(listGetNext(arguments))),
			height = atoi((char*)listGetNext(listGetNext(arguments)));
	char* matrix = (char*)listGetNext(listGetNext(arguments));
	return printErrorCode(yad3serviceRealtorAddApartment(yad3, email,
			service, id, price, width, height, matrix));
}


/*
 * interpreterRealtorRemoveApartment: assuming the command was relevant to
 * realtor, and the subcommand was to "remove_apartment", performs the function
 * and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterRealtorRemoveApartment(yad3service yad3, char* email,
		List arguments){
	char* service = (char*)listGetNext(listGetNext(arguments));
	int id = atoi((char*)listGetNext(listGetNext(arguments)));
	return printErrorCode(yad3serviceRealtorRemoveApartment(
			yad3, email, service, id));
}

/*
 * interpreterRealtorRespondToOffer: assuming the command was relevant to
 * realtor, and the subcommand was to "respond_to_offer", performs the function
 * and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterRealtorRespondToOffer(yad3service yad3, char* email,
		List arguments){
	char *customer_email = (char*)listGetNext(arguments),
			*str_choice = (char*)listGetNext(arguments);
	Choice choice;
	if(strcmp(str_choice, "accept") == 0)
		choice = accept;
	else if(strcmp(str_choice, "decline") == 0)
		choice = decline;
	else	// the string was not "accept" or "decline", so its unknown
		choice = unknown;
	return printErrorCode(yad3serviceRealtorRespondToOffer(yad3,
			email, customer_email, choice));
}

/*
 * interpretRealtorInput: assuming the command was relevant to realtor,
 * performs the subcommand on the list of arguments given
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */

static bool interpretRealtorInput(yad3service yad3, char* subcommand,
		List arguments){
	char* email = listGetFirst(arguments);
	if(strcmp(subcommand, "add") == 0){
		return interpreterRealtorAdd(yad3, email, arguments);
	}
	if(strcmp(subcommand, "remove") == 0){
		return printErrorCode(yad3serviceRemoveRealtor(yad3, email));
	}
	else if(strcmp(subcommand, "add_apartment_service") == 0){
		return interpreterRealtorAddService(yad3, email, arguments);
	}
	else if(strcmp(subcommand, "remove_apartment_service") == 0){
		return interpreterRealtorRemoveService(yad3, email, arguments);
	}
	else if(strcmp(subcommand, "add_apartment") == 0){
		return interpreterRealtorAddApartment(yad3, email, arguments);
	}
	else if(strcmp(subcommand, "remove_apartment") == 0){
		return interpreterRealtorRemoveApartment(yad3, email, arguments);
	}
	else if(strcmp(subcommand, "respond_to_offer") == 0){
		return interpreterRealtorRespondToOffer(yad3, email, arguments);
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}
// </Realtor commands>



// <Customer commands>

/*
 * interpreterCustomerAdd: assuming the command was relevant to
 * realtor, and the subcommand was to "add", performs the function
 * and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterCustomerAdd(yad3service yad3,char* email,
		List arguments){
	int area = atoi(listGetNext(arguments)),
			rooms = atoi((char*)listGetNext(arguments)),
			price = atoi((char*)listGetNext(arguments));
	return printErrorCode(yad3serviceAddCustomer(yad3, email,
			area, rooms, price));
}


/*
 * interpreterCustomerPurchase: assuming the command was relevant to
 * realtor, and the subcommand was to "purchase", performs the function
 * and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterCustomerPurchase(yad3service yad3, char* email,
		List arguments){
	char* realtor_email = (char*)listGetNext(arguments);
	char* service = (char*)listGetNext(arguments);
	int id = atoi((char*)listGetNext(listGetNext(arguments)));
	return printErrorCode(yad3serviceCustomerPurchase
			(yad3, email, realtor_email, service, id));
}


/*
 * interpreterCustomerMakeOffer: assuming the command was relevant to
 * realtor, and the subcommand was to "make_offer", performs the function
 * and returns whether or not the program should be closed
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpreterCustomerMakeOffer(yad3service yad3, char* email,
		List arguments){
	char* realtor_email = (char*)listGetNext(arguments);
	char* service = (char*)listGetNext(arguments);
	int id = atoi((char*)listGetNext(arguments)),
			new_price = atoi((char*)listGetNext(arguments));
	return printErrorCode(yad3serviceCustomerMakeOffer(yad3, email,
			realtor_email, service, id, new_price));
}

/*
 * interpretCustomerInput: assuming the command was relevant to customer,
 * performs the subcommand on the list of arguments given
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */

static bool interpretCustomerInput(yad3service yad3, char* subcommand,
		List arguments){
	char* email = listGetFirst(arguments);
	if(strcmp(subcommand, "add") == 0){
		return interpreterCustomerAdd(yad3, email, arguments);
	}
	else if(strcmp(subcommand, "remove") == 0){
		return printErrorCode(yad3serviceRemoveCustomer(yad3, email));
	}
	else if(strcmp(subcommand, "purchase") == 0){
		return interpreterCustomerPurchase(yad3, email, arguments);
	}
	else if(strcmp(subcommand, "make_offer") == 0){
		interpreterCustomerMakeOffer(yad3, email, arguments);
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}
// </Customer commands>




// <Report commands>
/*
 * interpretReportInput: assuming the command was relevant to report,
 * performs the subcommand on the list of arguments given
 *
 * @param yad3- the yad3service
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpretReportInput(yad3service yad3, char* subcommand,
		List arguments, FILE* output){
	if(strcmp(subcommand, "relevant_realtors") == 0){
		char* email = (char*)listGetFirst(arguments);
		return printErrorCode(yad3serviceReportRelevantRealtors(yad3, email,
				output));
	}
	else if(strcmp(subcommand, "most_paying_customers") == 0){
		int count = atoi((char*)listGetFirst(arguments));
		return printErrorCode(yad3serviceReportMostPayingCustomers
				(yad3, count, output));
	}
	else if(strcmp(subcommand, "significant_realtors") == 0){
		int count = atoi((char*)listGetFirst(arguments));
		return printErrorCode(yad3serviceReportSignificantRealtors
				(yad3, count, output));
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}

// </Report commands>


/*
 * interpretInput: checks who the command is relevant for, and calls
 * their interpretation function accordingly
 *
 * @param yad3- the yad3service
 * @param command- the command to be interpreted and performed
 * @param subcommand- the subcommand to be interpreted and performed
 * @param arguments- the list of arguments given by the user
 *
 * return- whether or not the program should be closed
 */
static bool interpretInput(yad3service yad3,
		char* command, char* subcommand, List arguments, FILE* output){
	LIST_FOREACH(char*, iterator, arguments){
		if(*(iterator+strlen(iterator)-1) == '\n'){
			*(iterator+strlen(iterator)-1) = '\0';
		}
	}
	if(strcmp(command, "realtor") == 0){
		return interpretRealtorInput(yad3, subcommand, arguments);
	}
	else if(strcmp(command, "customer") == 0){
		return interpretCustomerInput(yad3, subcommand, arguments);
	}
	else if(strcmp(command, "report") == 0){
		interpretReportInput(yad3, subcommand, arguments, output);
	}
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return true;
}

/*
 * string_copy: copies a string
 */

static char* stringCopy(char* string)
{
	int len=strlen(string);
	char* copy = malloc(len*sizeof(char*)+1);
	memset(copy, '\0', len);
	strcpy(copy, string);
	return copy;
}


/*
 * runLine: turns a line of input into a command, subcommand and list of
 * parameters, and sends them to be interpreted
 * if the interpreting function has found that the program needs to be stopped
 * returns a "true" value
 *
 * @param yad3 - the yad3service on which the actions will be performed
 * @param line - The line of input given to the interpreter.
 * @param output - the file to which reports will be printed
 *
 * return bool if the program needs to be closed
 */
static bool runLine(yad3service yad3, char* line, FILE* output){
	bool to_read = true, stop_running = false;
	char *word, *command, *subcommand;
	List arguments = listCreate((CopyListElement)stringCopy,
			(FreeListElement)strfree);
	int count = 0;
	word = strtok(line, "  \n");
	if(word == NULL)
		to_read = false;
	while(word != NULL){
		if(word == NULL){
			to_read = false;
			break;
		}
		if(count == 0){
			if(strchr(word, '#') != NULL){
				to_read = false;// if the word is a comment
				break;
			}
			command = word;
		}
		else if(count == 1)
			subcommand = word;
		else
			listInsertLast(arguments, word);
		count++;
		line += strlen(word);
		word = strtok(NULL," ");
	}
	if(to_read){
		stop_running = interpretInput(yad3, command, subcommand,
				arguments, output);
	}
	return stop_running;
}


/*
 * fileOpenedProperly: checks if the file was opened properly, meaning the
 * file isn't NULL. If file is NULL, then the program needs to be closed
 *
 * @param file: the file that should have been opened
 * @param close_program: whether or not we should close the program
 */
static void fileOpenedProperly(FILE* file, bool* close_program){
	if(file == NULL){
		mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
		*close_program = true;
	}
	else
		*close_program = false;
}

/*
 * printInvalidCommandLine: prints MTM_INVALID_COMMAND_LINE_PARAMETERS
 * and sets *close_program, meaning the program should be closed
 */
static void printInvalidCommandLine(bool* close_program){
	mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	*close_program = true;
}

static void runProgram(bool close_program, FILE* input, FILE* output){
	char* line = malloc(sizeof(char)*(MAX_LEN+1));
	memset(line, '\0', MAX_LEN+1);	// creates an empty string for fgets
	yad3service yad3 = yad3serviceCreate();
	while(fgets(line, MAX_LEN, input) != NULL && !close_program){
		close_program = runLine(yad3, line, output);
	}
	yad3serviceDestroy(yad3);
}


/*
 * main: checks where we should get the input and send the output, attempts to
 * open them accordingly, if it fails it prints an error and closes the program,
 * and if it succeeds it goes on to read every line of code in the input,
 * and perform it
 *
 * @param argc: the number of arguments given
 * @param argv: the array of words sent by the user, these will be turned
 * 				into arguments and performed accordingly
 */
int main(int argc, char** argv){
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	bool close_program;
	FILE *input = stdin, *output = stdout; // search standard input
	if(argc == 1){	// if input and output aren't given
	}
	else if(argc == 3){	 // if only input or only output are given
		if(strcmp(argv[1],"-i") == 0){
			input = fopen(argv[2], "r");
			fileOpenedProperly(input, &close_program);
		}
		else if(strcmp(argv[1],"-o") == 0){
			output = fopen(argv[3], "w");
			fileOpenedProperly(output, &close_program);
		}
		else
			printInvalidCommandLine(&close_program);
	}
	else if(argc == 5){	// if both input and output are given
		if(strcmp(argv[1],"-i") == 0){
			input = fopen(argv[2], "r");
			fileOpenedProperly(input, &close_program);
		}
		else if(strcmp(argv[1],"-o") == 0){
			output = fopen(argv[2], "w");
			fileOpenedProperly(output, &close_program);
		}
		else
			printInvalidCommandLine(&close_program);
		if(strcmp(argv[3],"-i") == 0){
			input = fopen(argv[3], "r");
			fileOpenedProperly(input, &close_program);
		}
		else if(strcmp(argv[3],"-o") == 0){
			output = fopen(argv[3], "w");
			fileOpenedProperly(output, &close_program);
		}
		else
			printInvalidCommandLine(&close_program);
	}
	else
		printInvalidCommandLine(&close_program);

	runProgram(close_program, input, output);
	return 0;
}



