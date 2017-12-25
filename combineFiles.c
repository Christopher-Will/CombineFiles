#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXSTDINSIZE 1024 * 1000 //user may only enter 100KB to stdin
#define _FILE_OFFSET_BITS 64 //use 64 bits for handling large files

//Convert the integer in -eN or -sN to digits, and return that value
unsigned long long optionToDigits(char option[]){
	unsigned long long optionLength = strlen(option);
	char digits[optionLength - 1];
	//copy the characters starting from 2nd index
	strncpy(digits, option + 2, optionLength - 2);
	digits[optionLength - 2] = '\0';
	return atoi(digits);
}

//Get the user's input from stdin and write it to stdout
void getStdinFile(char option[]){	
       //read in at most 100KB from stdin, or until the user enters CTRL + D
       char input[MAXSTDINSIZE + 1];
       char output[MAXSTDINSIZE + 1];
       while(fgets(input, MAXSTDINSIZE + 1, stdin)){
       		//add the current input to the char array containing the user's input
		strcat(output, input);
	}			        
	output[strlen(output) - 1] = '\0'; // add the NULL terminator to the end of the output array

	//Determine which option, -a, -sN, or -eN the user entered
	char firstTwoChar[3];
	strncpy(firstTwoChar, option, 2);
	firstTwoChar[2] = '\0';
	
	/*Variables to hold the starting and ending indices to show
	for the users file*/
	unsigned long long startByte;
	unsigned long long endByte;
	if(strcmp(option, "-a") == 0 ){
		//show the entire file
		startByte = 0;
		endByte = strlen(output);
	}else if(strcmp(firstTwoChar, "-s") == 0){
		startByte = 0;
		/*if user wishes to see more bytes then are in the file,
		set the endByte to the size of the char array*/
		if(optionToDigits(option) >= strlen(output)) endByte = strlen(output);
		else endByte = optionToDigits(option);
		
	}else{
		/*If user wishes to see more bytes then are in the file
		set the startByte to 0, else set it to the difference of
		the size of the char array and the N value given in -eN*/
		if(optionToDigits(option) >= strlen(output)) startByte = 0;
		else startByte = strlen(output) - optionToDigits(option);
		endByte = strlen(output);
	}	

	/*output the appropriate number of bytes, based on user
	option and size of char array*/
	unsigned long long currentByte;
	for(currentByte = startByte; currentByte < endByte; currentByte+=1){
		printf("%c", output[currentByte]);
	}
}

//Validate the user gave a valid file name
int validateFile(FILE *userFile){
	if(userFile == NULL) return 0;
	return 1;
}

//User entered -a, so write their entire file to stdout
void writeAllFile(FILE *userFile){
	/*Find total bytes in file by moving file pointer to
	the end of the file and using ftell()*/
	//See README for explanation of use of fseeko and ftello
	fseeko(userFile, 0, SEEK_END);
	unsigned long long userFileSize = ftello(userFile);
	fseeko(userFile, 0, SEEK_SET);
	//allocate space for char pointer to hold each byte of users file
	char *fileData;
	fileData = (char *) malloc(userFileSize + 1);
	
	/*Read each byte from the users file and store
	in the char pointer, then close their file */
	fread(fileData, userFileSize, 1, userFile);
	fclose(userFile);
	//Set the last index as the NULL terminator
	fileData[userFileSize] = '\0';
	//Write each byte in the users file to stdout
	fwrite(fileData, userFileSize, 1, stdout);
	free((char *) fileData);
}

/*User entered -sN, or eN so write the first or last N
bytes of their file to stdout*/
void writeNBytes(FILE *userFile, char option[]){
	//Find the N integer value given from -eN or -sN
	unsigned long long bytesToWrite = optionToDigits(option);
	/*Move the file pointer to the end of the file and use
	ftello() to get the total bytes in the file*/

	//See README for explanation of use of fseeko and ftello
	fseeko(userFile, 0, SEEK_END);
	unsigned long long userFileSize = ftello(userFile);
	/*If user wants more bytes then are in the file write
	the entire file to stdout*/
	if(bytesToWrite >= userFileSize){
		writeAllFile(userFile);
		return;
	}
	
	//char array to find whether the user entered -e or -s
	char firstTwoChar[3];
	strncpy(firstTwoChar, option, 2);
	firstTwoChar[2] = '\0';
	//allocate space for char pointer to hold each byte in users file 
	char *fileData;
	fileData = (char *) malloc(userFileSize + 1);

	/*User wants the first N bytes, so move file
	pointer to beginning of the file*/
	if(strcmp(firstTwoChar, "-s") == 0) fseek(userFile, 0, SEEK_SET);
	/*User wants to write the last N bytes, so move the
	file pointer to the end of the file and then offset it
	by N bytes */
	else fseek(userFile, -bytesToWrite, SEEK_END);
		
	/*Read the N bytes the user wishes to see into
	the char pointer and close the file */
	fread(fileData, bytesToWrite, 1, userFile);
	fclose(userFile);
	fileData[bytesToWrite] = '\0'; //set the NULL terminator
	//Write the N bytes to stdout
	fwrite(fileData, bytesToWrite, 1, stdout);
	free((char *) fileData);
}

//Validate whether the user entered -a, -sN, or -eN
int validateOption(char option[]){
	//array to hold the first 2 characters of the users option
	char firstTwoChar[3];
	strncpy(firstTwoChar, option, 2);
	firstTwoChar[2] = '\0';
	//get the total length of the users option
	unsigned long long optionLength = strlen(option);

	/*Return 0 for invalid option if user entered -a 
	with additional text after the -a*/
	if(strcmp(firstTwoChar, "-a") == 0){
		if(optionLength > 2) return 0;
		else return 1;//user only entered -a so return 1 for valid option
	
	/*Check whether first 2 characters of the option were -s or -e*/
	}else if(((strcmp(firstTwoChar, "-s") == 0)) || (strcmp(firstTwoChar, "-e") == 0)){
		//char array to hold all char's after the -s or -e
		char digits[optionLength - 1];
		/*fill digits[] with every character after the first 2
		from the users option*/
		strncpy(digits, option + 2, optionLength - 2);
		digits[optionLength - 2] = '\0';

		//Find the number of entries in the digits array
		unsigned long long digitsLength = strlen(digits);
		//Check if user only entered -s or -e, with no integer value
		if(digitsLength == 0) return 0;
		/*Iterate over the digits array, returning 1 for invalid option
		if any character is not a digit*/
		int currentDigit;
		for(currentDigit = 0; currentDigit < digitsLength; currentDigit+=1){
			if(!(isdigit(digits[currentDigit]))) return 0;
		}
		//Each character in digits[] was an integer so return 1 for valid option
		return 1;
	//Option wasn't -a, -sN, or -eN so return 0 for invalid option
	}else return 0;
}

int main(int argc, char *argv[]){
	int currentArg;//iterator for the current argument
	//Haven't gotten stdin yet so set to 0
	int alreadyGotStdin = 0;

	/*iterate over the users SPEC-FILE pairs, incrementing by
	2 each time to get the next pair*/
	for(currentArg = 1; currentArg < argc; currentArg+=2){
		char *currentOption = argv[currentArg];
		char *currentFile = argv[currentArg + 1];
	
		int isValidOption = validateOption(currentOption);
		//User didn't give valid option so print error and go to next pair
		if(isValidOption == 0){
			fprintf(stderr, "%s: Invalid option\n", currentOption);
			continue;
		}
	
		//User didn't give file for last option, so print error and exit the loop
		if(currentArg == argc - 1){
			fprintf(stderr, "Option '%s' requires accompanying file\n", currentOption);
			continue;
		}

		/*User gave valid option so check if they entered -
		to get input from stdin*/ 		
		if((strcmp(currentFile, "-") == 0)){
			/*Only get input from stdin the first time
			- is read*/
			if(alreadyGotStdin == 0){
				getStdinFile(currentOption);
				/*Set alreadyGotStdin to true so that 
				the user can only enter text into stdin once */
				alreadyGotStdin = 1;
			}
			continue;
		}
		/*User didn't enter - for their file, so validate whether 
		the given file is valid*/
		FILE *userFile;
		userFile = fopen(currentFile, "rb");
		int isValidFile = validateFile(userFile);
		/*User gave valid file so determine which 
		option they provided*/
		if(isValidFile == 1){
		//Get the 1st 2 characters from the users option
			char firstTwoChar[3];
			strncpy(firstTwoChar, currentOption, 2);
			firstTwoChar[2] = '\0';
			if(strcmp(firstTwoChar, "-a") == 0) writeAllFile(userFile);
			else writeNBytes(userFile, currentOption);
		//User gave invalid file so print error message
		}else fprintf(stderr, "%s: Error opening file\n", currentFile);
	}
	return 0;
}
