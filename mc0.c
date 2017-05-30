//Code by Carter Reynolds and Alazar Genene 1/18/17g
#include <sys/time.h>/** All the necessary libraries needed to  run the everything*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>

struct timeval printStats(struct timeval start);
int main(void){

	struct timeval start;//struct that has the start time 
	char *line=NULL;/*The input from the command line*/
	char *argus=NULL; 
	char *path=NULL;
	size_t len = 0;// required length of bytes read from getline
	ssize_t read;//bytes read from getline
	char command[80];// the system argument command for the first two commands in the menu
	while(1){
		printf("===== Mid-Day Commander, v0 =====\n");//first line of the command menu
		printf("G’day, Commander! What command would you like to run?\n ");//the greetings line
		printf("\t0. whoami: Prints out the result of the whoami command\n");//first command
		printf("\t1. last  : Prints out the result of the last command\n");//second command
		printf("\t2. ls    : Prints out the result of a listing on a user-specified path\n");//third available command
		printf("Option?: ");//prompts user to enter in an option
		while(getline(&line, &len, stdin)== -1);//gets user input and puts into line variable
		printf("\n\n");//two newlines for formatting
		gettimeofday(&start,NULL);//template for the statistics
		if(line[0]=='0')//if the user typed in 0
		{
			printf("-- Who Am I? --\n");//printf which command was chosen
			strcpy(command,"whoami");//copy string into command variable
			system(command);//use system to carry out said command
			printf("\n\n");//two newl lines again for format
		}
		else if(line[0]=='1')//if user types in 1 
		{
			printf("-- Last Logins --\n");//let user know option that was picked		
			strcpy(command,"last");//copy the string last into the command variable
			system(command);//use system to carry out command 
			printf("\n\n");//two newlines for format
		}
		else if(line[0]=='2')//if the user types in 2
		{
			printf("-- Directory Listing --\n");//lets them know which command they chose
			int numArgs=0;//num of args start at 0
			printf("Arguments?: ");//ask for the arguments
			read=getline(&argus, &len, stdin);//stores the number of arguments typed by user in read
			printf("Path?: ");//asks the user for the desired path
			read+=getline(&path, &len, stdin);//appends the path to the read variable
			char *args[read];//args a pointer to the args and path 
			char *save1,*save2;//the two delimiters
			if((args[numArgs]=strtok_r(argus," \n\r\t",&save1))!=NULL)//if strtok doesn't return Null it grabs the first string up until the first delimiter
			{
				numArgs++;	
				while((args[numArgs]=strtok_r(NULL," \n\r\t",&save1))!=NULL)//does the same thind until there are no spaces or tabs in the first delimiter
					numArgs++;
			}
			if((args[numArgs]=strtok_r(path," \n\r\t",&save2))!=NULL)//does the same thing only up to the second delimiter
			{
				numArgs++;		
				while((args[numArgs]=strtok_r(NULL," \n\r\t",&save2))!=NULL)
					numArgs++;
			}
			gettimeofday(&start,NULL);
			int rc=fork();//starts the child process
			if(rc < 0) {//if it's 0 then something went wrong
				printf("fork failed\n");
				exit(1);//exits program	
			}
			else if(rc == 0) {//if no children processes
				char *myargs[numArgs+2];//add 2 to the myargs[numargs] element
				myargs[0]=strdup("ls");//puts ls in first 2 elements of myargs
				for(int j=1;j<numArgs+1;j++)//loop to go through myargs
					myargs[j]=args[j-1];//put args-1 element in myargs
				myargs[numArgs+1]=NULL;//set the myargs[numArgs] to NULL
				printf("\n\n");//add two newlines
				execvp(myargs[0],myargs);//start a new excecution in myargs[0], myargs times
				printf("\n\n");//newlines for formating
			}
			else//otherwise wait for child processes to be done
				wait(NULL);
			
		}
		else
			printf("Invalid Input. Please try again.");//print invalid input if none of the propper characters commands were typed in
		printf("\n\n");//newlines for formant
		printStats(start);//print out stats
	}
	free(argus);//free variables so that they can be used again in the loop
	free(path);//
	free(line);//
	return 0;
}
	struct timeval printStats(struct timeval start)//printstats struct contents
	{
		struct timeval now;//needs a timeval
		struct rusage use;// and rusage
		gettimeofday(&now,NULL);//calls gettimeofday 
		int diff;//variable to find the time difference
		diff=(now.tv_sec-start.tv_sec)*1000;//subtract the beginning time from the end time and multiply by 1000 to make it more accurate
		int udiff=now.tv_usec-start.tv_usec;//udiff set to difference of now
		if(udiff<0)//if udiff is negative
			udiff+=1000000;// add a million to udiff
		udiff/=1000; //divide udiff by 1000
		diff+=udiff;//add up diff and udiff
		getrusage(RUSAGE_SELF,&use);//use getrusafe function
		printf("-- Statistics --\n");//print the stats
		printf("Elapsed time: %i milliseconds\n",diff);//elapsed time
		printf("Page Faults: %i\n",(int)use.ru_majflt);//page faults
		printf("Page Faults (reclaimed): %i\n\n\n",(int)use.ru_minflt);//pagefaults reclaimed
	}
