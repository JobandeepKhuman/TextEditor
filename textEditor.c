//The text editor program allows the user to create, edit, delete and manipulate
//text files, whilst keeping track of all the actions the user takes. The user is
//able to carry out 13 actions in total: 1=Create File, 2=Copy File,3=Delete File
//4=Show File, 5=Append Line to File, 6=Delete Line from File, 7=Insert Line into File
//8=Show Line in File, 9=Show Change Log, 10=Show Number of lines in File
//11=Find a line in a File, 12=Replace a line in a File, 13=Exit Program. Each
//comand is handled by a seperate function/procedure and all user input is validated
//and appropriate error messages are output when invalid input is entered.


//Linking neccessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function to check if the lineNumber input by user is an integer less than the
//total number of lines in the file
//A pointer to the first character in the filename input and a pointer to the first
//character of the lineNumber input are taken as parameters
//The function returns 1 if the lineNumberInput is valid and 0 if it is not valid
//Error messages are output to stdout to explain to the user why their input is invlaid
int validLineChecker(char *filename, char* lineNumberInput){
	//Checking if the lineNumber entered was an integer
	//Initialising variables
  int counter;
	int test=1;//1 implies input is valid; 0 implies input is invalid
  int lineTracker=0;
	//Calculating number of characters in input
	int length=strlen(lineNumberInput);
  //Looping through each character in the string
	for(counter=0;counter<length;counter++){
    //Creating a pointer variable that will point at each character in the string as
    //the loop continues, created to make the following line more easily readable
		char c=*(lineNumberInput+counter);
		//Checking if any character in lineNumberInput is not a numerical character
		if(c!='0'& c!='1'& c!='2'& c!='3'& c!='4'& c!='5'& c!='6'& c!='7'& c!='8'& c!='9'){
			//Setting test to 0 if a non numerical character is found to signify the
      //lineNumberInput is not valid
			test=0;
    }
  }
	//Outputting approproate error message if lineNumberInput is not an integer
	if(test==0){
		printf("ERROR, lineNumber must be an integer\n");
  }
	//If lineNumberInput is an integer check it is less than the total number of lines in the file
	else{
		//Opening file to count how many lines it has
    FILE *filePointer;
		filePointer=fopen(filename, "r");
		//Initialising the character variable that temporarily holds each character in the file
		char character=fgetc(filePointer);
		//Looping through until the end of the file
		while(character!=EOF){
			//Incrementing the lineTracker variable whenever a newline character is	found
			if(character=='\n'){
				lineTracker++;
      }
			character=fgetc(filePointer);
    }
    //Converting lineNumber to an integer
    int integerLineNumber=atoi(lineNumberInput);
    //Outputting an error message if the lineNumberInput is an integer greater than the
    //total number of lines in the file
		if(integerLineNumber>lineTracker){
			printf("ERROR, the lineNumber input exceeds the total number of line in the file %s only has %d lines\n",filename,lineTracker);
      test=0;//Updating test variable to signify that the lineNumberInput is not valid
    }
    if(integerLineNumber==0){
      printf("Line numbers start from 1, therefore 0 is an invalid lineNumber\n");
      test=0;
    }
  }
	return test;//Returning result of validation function
}

//Procedure to update the changeLog after a command is run
//Takes a pointer pointing to the fisrt character of the name of the command, a
//file pointer pointing to the file and a pointer pointing to the first character
//of the name of the file as input parameters
//Updates the changeLog file by appending a line that states the command carried
//out, the file it was carried out on, and the number of lines the file is left
//with once the command is carried out.
void addChangeLog(char* command, FILE* filePointer, char* filename){
  //Initialising variables
  int lineTracker=0;
  char logInput[324];
  FILE *logPointer;
  //Pointing the filePointer to the start of the file
  rewind(filePointer);
  //Opening files in the correct modes
	logPointer=fopen("changeLog","a+");
  //Initialising variables
	char newCharacter=fgetc(filePointer);
  //Looping through the file to calculate how many lines it has
  //Checking if the file is empty
  if(ftell(filePointer)!=0){
    //Counting the number of lines in the file
    //Looping through each character in the file
  	while(newCharacter!=EOF){
      //Incrementing lineTracker once a newline character is found
  		if(newCharacter=='\n'){
  			lineTracker++;
      }
      //Updating newCharacter to the equal the next character in the file
  		newCharacter=fgetc(filePointer);
    }
  }
  //Creating a string to write to the changeLog
  sprintf(logInput,"%s has been run on %s leaving the file with %d lines",command,filename,lineTracker);
  fprintf(logPointer,"\n%s",logInput);//Writing the string to the changeLog file
  fclose(logPointer);//Closing the changeLog file
  fclose(filePointer);//Closing the file the operation was carried out on
}

//Declaring the createFiie procedure, if valid input is supplied the procedure
//creates a new file with the filename input
//Takes no parameters but recieves the filename as input from the user
//Outputs success or error message if the filename is valid or invalid respectivley
void createFile(){
  //Prompting the user for input
  printf("Please enter the name of the file you would like to create\n");
  char filename[255];
  scanf("%s", filename);
  FILE *check;
  check=fopen(filename,"r");
  //Creating the file if it doesnt already exist
  if(check==NULL){
    //Opening the file in write mode creates the file
    check=fopen(filename,"w+");
    printf("The file was successfully created\n");
    //Updating changeLog
    char command[]="createFile";
    addChangeLog(command, check, filename);
  }
  //Outputting appropriate error message if the file already exists
  else{
    printf("A file called %s already exists in the current directory, please use a different name\n",filename);
  }
}

//Declaring the copyFiie procedure, copies a file to another file if valid input is
//entered by the user, otherwise an appropriate error message is displayed
//Recieves the name of the file to be copied and the name of the file the contents
//will be copied to as ipnut from the user
void copyFiie(){
  //Asking for and recieving user input
  printf("Please enter the name of the file you would like to copy\n");
  char sourceName[255];
  scanf("%s",sourceName);
  printf("Please enter the name of the file you would like to create\n");
  char destinationName[100];
  scanf("%s",destinationName);

  //Initialising pointers in read mode, because read mode returns NULL if a file does not exist
  FILE *destinationPointer;
  destinationPointer=fopen(destinationName,"r");
  FILE *sourcePointer;
  sourcePointer=fopen(sourceName,"r");

  //Copying the original and creating the new file if the original file exists in
  //the current directory and the new file name is not in use in the current directory
  if(sourcePointer!=NULL & destinationPointer==NULL){
    //Opening the destination file in write mode
    destinationPointer=fopen(destinationName,"w+");
    //Looping through each character in the original file and writing it to the new file
    char character=fgetc(sourcePointer);
    while(character!=EOF){
      //Writing each character to the file
      fputc(character,destinationPointer);
      //Updating the character
      character=fgetc(sourcePointer);
    }
    printf("%s has been copied to %s\n",sourceName, destinationName);
    fclose(sourcePointer);;
    //Updating changeLog
    char command[]="copyFile";
    addChangeLog(command, destinationPointer, destinationName);
  }
 //Outputting approproate error message, explaining why the input was invalid
 else if(sourcePointer==NULL & destinationPointer==NULL){
   printf("ERROR, the file you would like to copy does not exist in the current directory\n");
 }
 //Outputting approproate error message, explaining why the input was invalid
 else if(sourcePointer==NULL & destinationPointer!=NULL){
   printf("ERROR, the file you would like to copy does not exist in the current directory and\n");
	 printf("the name of the file you would like to create is in use by a different file in the current directory\n");
   fclose(destinationPointer);
 }
 //Outputting approproate error message, explaining why the input was invalid
 else{
   printf("ERROR, the name of the file you would like to create is in use by a different file in the current directory\n");
   fclose(sourcePointer);
   fclose(destinationPointer);
 }
}

//Procedure to delete a given file
//Filename is recieved as input from the user and if a file of that name exists in
//the current directory it is deleted, otherwise an appropriate error message is
//displayed
void deleteFile(){
  //Initialising the filename variable
  char filename[255];
  //Asking for and recieving filename input
  printf("Please enter the name of the file you would like to delete\n");
  scanf("%s",filename);
  //Opening file to see if it is in current directory
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  //Deleting file if it is in current directory
  if(filePointer!=NULL){
    remove(filename);
    printf("%s has been deleted\n",filename);
    //Updating changeLog
    char command[]="deleteFile";
    addChangeLog(command, filePointer, filename);
  }
  else{
    //Outputting appropriate error message
    printf("ERROR, the file you would like to delete does not exist in the current directory\n");
  }
}

//Procedure to show a given file
//Filename is recieved as input from the user and if the file exists in the current
//directory it is displayed to the user, otherwise an error message is displayed
void showFile(){
  //Initialising the filename variable
  char filename[255];
  //Asking for and recieving filename input
  printf("Please enter the name of the file you would like to display\n");
  scanf("%s",filename);
  //Opening file to see if it is in current directory
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  //Displaying file if it is in current directory
  if(filePointer!=NULL){
    //Initialising variable
    char character=fgetc(filePointer);
    //Looping through each character in the file
    while(character!=EOF){
      //Displaying the chracter
      printf("%c",character);
      //Updating the character
      character=fgetc(filePointer);
    }
    //Make sure the menu is displayed on a different line to the last line of the file
    printf("\n");
  }
  else{
    //Outputting appropriate error message
    printf("ERROR, the file you would like to display does not exist in the current directory\n");
  }
}

//Procedure to add a line to the end of a given file
//Filename and the line to be entered are recieved as input from the user, if the
//file exists in the current directory the line is appended to the end of the file
//otherwise an appropriate error message is displayed
void appendLine(){
  //Initialising the filename variable
  char filename[255];
  //Asking for and recieving filename input
  printf("Please enter the name of the file you would like to add a line to\n");
  scanf("%s",filename);
  //Opening file to see if it is in current directory
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  //Adding line if file if it is in current directory
  if(filePointer!=NULL){
    filePointer=fopen(filename,"a+");
    //Initialising variable
    char line[800];
    //Asking for and recieving line input
    printf("Please enter the line you would like to add\n");
    scanf("%s",line);
    //Prints the line input to the file
    fprintf(filePointer,"%s",line);
    fputs("\n",filePointer);
    //Updating changeLog
    char command[]="appendLine";
    addChangeLog(command, filePointer, filename);
  }
  else{
    //Outputting appropriate error message
    printf("ERROR, the file you would like to add a line to does not exist in the current directory\n");
  }
}

//Procedure to delete a line from a given file
//Filename and the lineNumber to be deleted are recieved as input from the user, if the
//file exists in the current directory and the lineNumber is valid the line is removed
//from the file otherwise an appropriate error message is displayed
void deleteLine(){
  //Initialising variables
  char filename[255];
  char lineNumberInput[7];
  //Asking for and recieving filename input
  printf("Please enter the name of the file you would like to remove a line from\n");
  scanf("%s",filename);
  //Asking for and recieving line number input
  printf("Please enter the lineNumber of the line you would like to delete\n");
  scanf("%s",lineNumberInput);
  //Opening file to see if it is in current directory
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  if(filePointer!=NULL){
    //Validating the lineNumber input by the user
    int test=validLineChecker(filename, lineNumberInput);
    //Checking if a valid lineNumber was input
    if(test==1){
      //Converting the lineNumber input by the user from a string to an integer
      int lineNumber=atoi(lineNumberInput);
      //Decrementing line number because the first line does not start with a newline character
      lineNumber--;
      //Initialising temporaryFile pointer
      FILE *tempFilePointer;
      tempFilePointer=fopen("temporaryFile","w+");
      //Initialising character variable
      char character=fgetc(filePointer);
      //Copying the file to temporaryFile, except for the line to be deleted
      //Looping through each chracter
      while(character!=EOF){
        //Writing the characters of the file that are not on the line enumber to be deleted
        //to the tmeporaryFile
        if(lineNumber!=0){
          fputc(character, tempFilePointer);
        }
        //Decrementing lineNumber when a newline chracter is found
        if(character=='\n'){
          lineNumber--;
        }
        //Updating the chracter
        character=fgetc(filePointer);
      }
      //Deleting the file once it has been copied
      remove(filename);
      FILE *newFilePointer;
      newFilePointer=fopen(filename,"w+");
      //Pointing the temporaryFile pointer back to the start of temporaryFile
      rewind(tempFilePointer);
      //Setting chracter to the first chracter in temporaryFile
      character=fgetc(tempFilePointer);
      //Looping through each character in temporaryFile
      while(character!=EOF){
        //Writing the chracter to the newFile
        fputc(character,newFilePointer);
        //Updating the chracter
        character=fgetc(tempFilePointer);
      }
      //Deleting the temporaryFile once it has been copied back to the original file
      remove("temporaryFile");
      //Updating changeLog
      char command[]="deleteLine";
      addChangeLog(command, newFilePointer, filename);
      }
    }
    else{
      //Outputting appropriate error message
      printf("ERROR, the file you would like to remove a line from does not exist in the current directory\n");
    }
}


//Procedure to insert a line into a given file
//Filename, the lineNumber at which to insert the line and the line to be entered
//are recieved as input from the user If the file exists in the current directory
//and the lineNumber is valid the line is inserted to the file otherwise an
//appropriate error message is displayed
void insertLine(){
  //Initialising the filename variable
  char filename[255];
  //Asking for and recieving filename input
  printf("Please enter the name of the file you would like to insert a line into\n");
  scanf("%s",filename);
  //Opening file to see if it is in current directory
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  //Adding line if file if it is in current directory
  if(filePointer!=NULL){
    //Asking for and recieving line number input
    printf("Please enter the lineNumber of the line you would like to insert\n");
    char lineNumberInput[7];
    scanf("%s",lineNumberInput);
    //Checking if the lineNumber input is valid
    if(validLineChecker(filename,lineNumberInput)==1){
      //Converting lineNumberInput to an integer
      int lineNumber=atoi(lineNumberInput);
      //Decrementing line number because the first line does not start with a newline character
      lineNumber--;
      //Asking for and recieving the line to be inserted
      printf("Please enter the line you would like to insert\n");
      char line[800];
      scanf("%s",line);
      //Initialising temporaryFile pointer
      FILE *tempFilePointer;
      tempFilePointer=fopen("temporaryFile","w+");
      //Initialising character variable
      char character=fgetc(filePointer);
      //Copying the file to temporaryFile and adding the extra line in the correct position
      //Looping through each chracter
      while(character!=EOF){
        //Writing the extra line to the file
        if(lineNumber==0){
          fputs(line, tempFilePointer);
          fputs("\n",tempFilePointer);
          lineNumber--;
        }
        //Decrementing lineNumber when a newline chracter is found
        if(character=='\n'){
          lineNumber--;
        }
        //Writing the character to the temporaryFile
        fputc(character, tempFilePointer);
        //Updating the chracter
        character=fgetc(filePointer);
      }
      //Deleting the file once it has been copied
      remove(filename);
      FILE *newFilePointer;
      newFilePointer=fopen(filename,"w+");
      //Pointing the temporaryFile pointer back to the start of temporaryFile
      rewind(tempFilePointer);
      //Setting chracter to the first chracter in temporaryFile
      character=fgetc(tempFilePointer);
      //Looping through each character in temporaryFile
      while(character!=EOF){
        //Writing the chracter to the newFile
        fputc(character,newFilePointer);
        //Updating the chracter
        character=fgetc(tempFilePointer);
      }
      //Deleting the temporaryFile once it has been copied back to the original file
      remove("temporaryFile");
      //Updating changeLog
      char command[]="insertLine";
      addChangeLog(command, newFilePointer, filename);
    }
  }
  else{
    //Outputting appropriate error message
    printf("ERROR, the file you would like to insert a line into does not exist in the current directory\n");
  }
}

//Procedure to show a line from a given file
//Filename and the lineNumber to be displayed are recieved as input from the user, if the
//file exists in the current directory and the lineNumber is valid the line  is displayed
//to the user otherwise an appropriate error message is displayed
void showLine(){
  //Initialising the filename variable
  char filename[255];
  //Asking for and recieving filename input
  printf("Please enter the name of the file you would like to display a line from\n");
  scanf("%s",filename);
  //Opening file to see if it is in current directory
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  //Adding line if file if it is in current directory
  if(filePointer!=NULL){
    //Asking for and recieving line number input
    printf("Please enter the lineNumber of the line you would like to show: ");
    char lineNumberInput[7];
    scanf("%s",lineNumberInput);
    if(validLineChecker(filename,lineNumberInput)==1){
      //Converting the lineNumber input to an integer
      int lineNumber=atoi(lineNumberInput);
      //Decrementing line number because the first line does not start with a newline character
      lineNumber--;
      //Initialising character variable
      char character=fgetc(filePointer);
      //Copying the file to temporaryFile and adding the extra line in the correct position
      //Looping through each chracter
      while(character!=EOF){
        //Outputting the characters on the line specified
        if(lineNumber==0){
          printf("%c",character);
        }
        if(character=='\n'){
          lineNumber--;
        }
        //Updating the chracter
        character=fgetc(filePointer);
      }
      //Starting a newline once the line has been displayed
      printf("\n");
    }
  }
  else{
    //Outputting appropriate error message
    printf("ERROR, the file you would like to display a line from does not exist in the current directory\n");
  }
}

//Procedure to display the changeLog
//The contents of the file named "changeLog" are output to the user
void showChangeLog(){
  //Opening the changeLog
  FILE *filePointer;
  filePointer=fopen("changeLog","r");
    //Displaying teh changeLog if it has any contents
    if(filePointer!=NULL){
    //Initialising character
    char character=fgetc(filePointer);
    //Looping through each character in the changeLog
    while(character!=EOF){
      //Displaying the chracter
      printf("%c",character);
      //Updating the character
      character=fgetc(filePointer);
    }
    //Closing the file
    fclose(filePointer);
  }
  else{
    printf("The changeLog is empty, no files have been edited");
  }
    //Make sure the menu is displayed on a different line to the last line of the file or error message
  printf("\n");
}

//Procedure to show the number of lines in a given file
//Filename is recieved as input from the user, if the file exists in the current
//directory the number of lines in the file is output otherwise an appropriate
//error message is displayed
void showLineNumber(){
  //Asking for and receiving the name of the file
  printf("Please enter the name of the file whose number of lines you would like to output\n");
  char filename[255];
  scanf("%s",filename);
  //Initialising the filePointer
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  //Running the command if the file is in the current directory
  if(filePointer!=NULL){
    //initialising variable
    int lineTracker=0;
  	//Initialising the character variable that temporarily hold each character in the file
  	char character=fgetc(filePointer);
  	//Looping through until the end of the file
  	while(character!=EOF){
  		//Incrementing the lineTracker variable whenever a newline character is found
  		if(character=='\n'){
  			lineTracker++;
      }
  		character=fgetc(filePointer);
     }
  	//Outputting the number of lines in the file
  	printf("%s is %d lines long\n",filename,lineTracker);
  	fclose(filePointer);
}
//Outputting an appropriate error message if the file is not in the current directory
else{
	printf("ERROR, the file you entered does not exist in the current directory");
  }
}

//Function to find each occurence of a string in a file
//Recieves a pointer to the first character of the file name and a pointer to the first
//character of the string to be searched for by the user as input. If the file exists
//in the current directory the lineNumber and the number of characters from the
//start of the file at which each occurence of the string can be found is output
//to the user. Additionally, an array of integers that tell the program after how many
//characters each occurence of the string occurs is returned by the program. If the
//file does not exist in the current directory an appropriate error message is displayed
int* stringFinder(char* filename,char* text){
	//Initialising the filePointer
  FILE *filePointer;
	filePointer=fopen(filename,"r");
  //pointerArray is a pointer to a collection of filePointers
  static int textLocations[800];
	//Running the command if the file is in the current directory
	if(filePointer!=NULL){
		//Initialising variables
		char character=getc(filePointer);
		int checker,counter,i,lineTracker=0;
    int characterTracker=1;
    FILE *startPointer;
		//Looping through each character in the file
		while(character!=EOF){
			//Checking if the current character is the current character in the text string
			if(character==*(text+checker)){
				//Updating the current character in the text string
				checker=checker+1;
      }
			//Resetting the current character in the text string to the start of the string if
      //no match was made on the current check
			else if(character!='\n'){
				checker=0;
      }
			//If every character in the text string has been matched consecutively, letting
    	//the user know a match has been found and what line it has been found on
			if(checker==strlen(text)){
				printf("%s has been found on line %d\n",text,lineTracker);
        //Storing the location of the first chracter of the matched string in textLocations
				textLocations[counter]=characterTracker-strlen(text);
        printf("The first chracter is on character %d\n",characterTracker-strlen(text));
        counter++;
        //Resetting the checker once a string has been matched
        checker=0;
      }
			//Updating the lineTracker if a newline character is found
			if(character=='\n'){
				lineTracker++;
      }
      //Updating the chracter variable
      character=getc(filePointer);
      characterTracker++;
      }
    }
	//Outputting an appropriate error message
	else{
		printf("ERROR, the file you entered does not exist in the current directory\n");
  }
  return textLocations;
}

//Procedure to replace each occurence of a given string in a file with a different string.
//Filename, the string to be replaced and the string that will replaced the other string
//are input by the user. If the file exists in the current directory each occurence of
//the string to be replaced is found by calling the stringFinder function and then
//it is replaced by the new string. If the file does not exist in the current directory
//an appropriate error message displayed
void stringReplacer(){
  //Asking for and receiving user input
  printf("Please enter the name of the file you would like to edit ");
  char filename[255];
  scanf("%s",filename);
  printf("Please enter the string you would like to replace ");
  char oldText[800];
  scanf("%s",oldText);
  printf("Please enter the string you would like to replace the old string with ");
  char newText[800];
  scanf("%s",newText);
  //Initialising the filePointer
  FILE *filePointer;
  filePointer=fopen(filename,"r");
  //Running the command if the file is in the current directory
  if(filePointer!=NULL){
    //Initialising variables
    char character=fgetc(filePointer);
    int i=0;
    int characterTracker=0;
    printf(" \n");
  	int* textLocations=stringFinder(filename, oldText);
    //printf("Function running\n");
  	//Initialising temporary file pointer
    FILE *tempFilePointer;
  	tempFilePointer=fopen("temporaryFile","w+");
    //Copying all the file contents to a temporary file and replacing all occurrences of oldText 	with newText
  	while(character!=EOF){
      int location= *(textLocations+i);
  		//Replacing the occurrence of oldText with newText
  		if(characterTracker == location){
    		//Writing newText where oldText used to be
    		fputs(newText, tempFilePointer);
  			//Incrementing the array counter so that it points to the pointer that holds the next occurrence of oldText
  			i++;
        //Updating the tempFilePointer so that it skips over the oldText characters
    		for(int j=0;j<strlen(oldText);j++){
          character=fgetc(filePointer);
        }
        //Updatin characterTracker so that it skips over oldText chracters
        characterTracker+=strlen(oldText);
      }
  		//Writing the next character to the temporaryFile
  		fputc(character, tempFilePointer);
      //Updating variables
      character=fgetc(filePointer);
      characterTracker++;
    }
  	//Deleting the file once it has been copied
  	remove(filename);
  	//Pointing the tempFilePointer to the start of the temporaryFile
  	rewind(tempFilePointer);
  	//Creating a new file with the name of the file that was copied
    FILE *newFilePointer;
  	newFilePointer=fopen(filename,"w");
  	//Copying the contents of the temporaryFile to the new file
    character=fgetc(tempFilePointer);
  	while(character!=EOF){
  		fputc(character, newFilePointer);
      character=fgetc(tempFilePointer);
    }
  	//Deleting the temporary file once the new file has been made
  	remove("temporaryFile");
  	printf("All occurrences of %s have been replaced with %s\n",oldText,newText);
  	//Updating the changeLog
  	//addChangeLog("stringReplace",newFilePointer,filename);
  	fclose(newFilePointer);
  }
  //Outputting an appropriate error message
  else{
  	printf("ERROR, the file you entered does not exist in the current directory");
  }
}

//The main function of the program will created the changeLog file if it does not
//already exist. Then a menu is displyed to the user and the user's commandCode is
//recieved, depending on the value of the commandCode a different function/procedure
//and hence a different command is appllied and then the process repeats until
//the exit commandCode '13' is entered by the user. If an invalid commandCode is
//entered by the user an appropriate error message is displayed.
int main(int argc, char *argv[]){
  //Telling the user what filenames not to use
  printf("Do not create a file called 'temporaryFile'  or 'changeLog'\n");
  //initialising variables
  char commandCode[2];
  char changeLog[]="changeLog";
  FILE *logPointer;
  logPointer=fopen("changeLog","r");
  //Creating the changeLog file if it does not already exist
  if(logPointer==NULL){
    logPointer=fopen("changeLog","w");
  }
  fclose(logPointer);
  //Running the program until the exit commandCode is entered
  while(strncmp(commandCode,"13",2)!=0){
    //Displaying the menu
    printf("Please Enter an command code from 1 to 11:\n");
    printf("01=Create File\n");
    printf("02=Copy File\n");
    printf("03=Delete File\n");
    printf("04=Show File\n");
    printf("05=Append Line to File\n");
    printf("06=Delete Line from File\n");
    printf("07=Insert Line into File\n");
    printf("08=Show Line in File\n");
    printf("09=Show Change Log\n");
    printf("10=Show Number of lines in File\n");
    printf("11=Find a string in a File\n");
    printf("12=Replace a string in a File\n");
    printf("13=Exit Program\n");
    //Receiving user input
    scanf("%s", commandCode);
    //Calling the procedure/function that corresponds to the commandCode input
    //by the user
    if(strncmp(commandCode,"01",2)==0){
      createFile();
    }
    else if(strncmp(commandCode,"02",2)==0){
      copyFiie();
    }
    else if(strncmp(commandCode,"03",2)==0){
      deleteFile();
    }
    else if(strncmp(commandCode,"04",2)==0){
      showFile();
    }
    else if(strncmp(commandCode,"05",2)==0){
      appendLine();
    }
    else if(strncmp(commandCode,"06",2)==0){
      deleteLine();
    }
    else if(strncmp(commandCode,"07",2)==0){
      insertLine();
    }
    else if(strncmp(commandCode,"08",2)==0){
      showLine();
    }
    else if(strncmp(commandCode,"09",2)==0){
      showChangeLog();
    }
    else if(strncmp(commandCode,"10",2)==0){
      showLineNumber();
    }
    else if(strncmp(commandCode,"11",2)==0){
      //Initialising variables
      char filename[255];
      char text[800];
      //Asking for and recieving user input
      printf("Please enter the name of the file you would like to search \n");
      scanf("%s",filename);
      printf("Please enter the string you would like to search for \n");
      scanf("%s",text);
      stringFinder(filename,text);
    }
    else if(strncmp(commandCode,"12",2)==0){
      stringReplacer();
    }
    //Informing the user the program will end so that they know the exit commandCode
    //was both entered and successfully carried out
    else if(strncmp(commandCode,"13",2)==0){
      printf("The program will now be terminated");
    }
    //Outputting an appropriate error message if an invalid commandCode is entered
    else{
      printf("An invalid command code was entered, please enter an integer from 1 to 11 inclusive\n");
    }
  }
}
