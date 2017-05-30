//Code by Carter Reynolds and Alazar Genene 1/18/17
#include <sys/time.h>/** all the libraries needed for it to work*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_INPUT (120)//max inputs of commands

struct timeval printStats(struct timeval start);
int main(void){

    struct timeval start;
    char option;
    char *line=NULL;
    char *argus=NULL;
    char *path=NULL;
    char *path2=NULL;
    size_t len = 0;
    ssize_t read;
    char command[80];
	int numRead;            
    char cmd[MAX_INPUT][128];//double pointer of typed commands 
    int counter = 2;//counter that keeps track of commands added
    while(1){
        printf("===== Mid-Day Commander, v0 =====\n");
        printf("G’day, Commander! What command would you like to run?\n ");
        printf("\t0. whoami: Prints out the result of the whoami command\n");
        printf("\t1. last  : Prints out the result of the last command\n");
        printf("\t2. ls    : Prints out the result of a listing on a user-specified path\n");
        if(counter > 2)//this part is to print the out the user commands
        	for(int i = 3; i <= counter; i++)
        		printf("\t%d. %s: user added command\n", i, cmd[i-1]);//add commands
        printf("\ta. add command   : Adds new a new command to the menu\n");//function that adds commands to the array of ptrs
        printf("\tc. change directory: Changes process working directory\n");//command to change the dir
        printf("\te. exit   : leave midday commander\n");//command to exit
        printf("\tp. pwd : prints working directory\n");//command to print working directory
        printf("Option?: ");//ask for option
        while(getline(&line, &len, stdin)== -1);
        printf("\n\n");
        gettimeofday(&start,NULL);
        if(line[0]=='0')
        {
            printf("-- Who Am I? --\n");
            strcpy(command,"whoami");
            system(command);
            printf("\n\n");
        }
        else if(line[0]=='1')
        {
            printf("-- Last Logins --\n");       
            strcpy(command,"last");
            system(command);
            printf("\n\n");
        }
        else if(line[0]=='2')
        {
            printf("-- Directory Listing --\n");
            int numArgs=0;
            printf("Arguments?: ");
            read=getline(&argus, &len, stdin);
            printf("Path?: ");
            read+=getline(&path, &len, stdin);
            char *args[read];
            char *save1,*save2;
            if((args[numArgs]=strtok_r(argus," \n\r\t",&save1))!=NULL)
            {
                numArgs++;       
                while((args[numArgs]=strtok_r(NULL," \n\r\t",&save1))!=NULL)
                    numArgs++;
            }
            if((args[numArgs]=strtok_r(path," \n\r\t",&save2))!=NULL)
            {
                numArgs++;       
                while((args[numArgs]=strtok_r(NULL," \n\r\t",&save2))!=NULL)
                    numArgs++;
            }
            gettimeofday(&start,NULL);
            int rc=fork();
            if(rc < 0) {
                printf("fork failed\n");
                exit(1);   
            }
            else if(rc == 0) {
                char *myargs[numArgs+2];
                myargs[0]=strdup("ls");
                for(int j=1;j<numArgs+1;j++)
                    myargs[j]=args[j-1];
                myargs[numArgs+1]=NULL;
                printf("\n\n");
                execvp(myargs[0],myargs);
                printf("\n\n");
            }
            else
                wait(NULL);
        }
        else if(line[0]=='e'||line[0]==EOF){//check if char in cm line is either EOF or e then exits
            printf("Logging you out Commmander.\n");//prints the statement 
            exit(1);//exits
        }
        else if(line[0]=='a'){//if char is a 
            printf("--Add a command--\n");//statement
            printf("Command to add?: ");//ask for user input
			while(numRead=getline(&path2,&len,stdin)==-1);//get the input from stdin and stick it in numRead
			if(numRead>128)//if you enter more characters than alloted
			{
				printf("Invalid Input. Must have less than 128 characters");//prints error and breaks avoiding seg faults
				break;
			}
			strtok(path2,"\n");//get rid of the newlines
            strncpy(cmd[counter], path2,100);//copy the cmd[counter] into path 2
            counter++;//increment counter because command was added
			printf("Okay, added with ID %i!",counter);  //print statement
        }   
        else if(line[0]=='p'){
            char cwd[1024];// allocate size of current working directory
               if (getcwd(cwd, sizeof(cwd)) != NULL)//gets current working directory with the command and makes sure it isn't NULL
                       fprintf(stdout, "--Current Directory--\n Directory: %s\n", cwd);//prints out the current working Directory to stdout
               else
                       perror("getcwd() error");//otherwise gcwd couldn't find it
        }
        else if(line[0]=='c'){
            printf("-- Change Directory --\n");//print statement 
            printf("New Directory?: ");// asks for new directory
            char dir[128];//max dir char lenght
            scanf("%s", dir);//scan the input put it into dir
            chdir(dir);//change dir to dir
            char c;//char to clear the buffer
            while(c = getchar()!='\n'&& c != EOF);//clear the buffer
        }
        else
        {
			int flag=1;//flag
			for(int i=3;i<=counter;i++)//goes through all of the enteries
				if(line[0]==(i+'0'))//if line[0] = 1
				{
        			char *save3;//save1
					char * copy = (char *)malloc(strlen(cmd[i-1]));//make a copy with len of cmd i-1
					int num=0;//make num 0
					char * args2[128];//char of args size of elements alotted
					strcpy(copy,cmd[i-1]);//copy it in the copy
        			if((args2[num]=strtok_r(copy," \n\r\t",&save3))!=NULL)//get rid of the tabs and newlines up to save3
        			{
					    num++;//increment num variable      
					    while((args2[num]=strtok_r(NULL," \n\r\t",&save3))!=NULL)//Do the same thing for  NULL
					        num++;
					}
					if(num>32)//if num is greater thn 32
					{
						printf("Invalid Input. Must have less than 32 Arguments");//exit because too many args
						break;
					}
					gettimeofday(&start,NULL);
					int rc=fork();
					if(rc < 0) {
					    printf("fork failed\n");
					    exit(1);   
					}
					else if(rc == 0) {
					    char *myargs2[num+1];
					    for(int j=0;j<num;j++)
					        myargs2[j]=args2[j];
					    myargs2[num]=NULL;
					    printf("\n");
					    execvp(myargs2[0],myargs2);
					    printf("\n");
					}
					else
					    wait(NULL);//wait for child processes
					flag=0;//else flag is 0
					free(copy);//free the copy
				}
			if(flag)//this makes sure the flag is niether 0, or 1 something went wrong
	    		printf("Invalid Input. Please try again.");
		}
        printf("\n\n");//new line for format
        printStats(start);
			
    }
    free(argus);//free all the variables
    free(path);
    free(line);
    free(path2);
    return 0;
}
    struct timeval printStats(struct timeval start)
    {
        struct timeval now;
        struct rusage use;
        gettimeofday(&now,NULL);
        int diff;
        diff=(now.tv_sec-start.tv_sec)*1000;
        int udiff=now.tv_usec-start.tv_usec;
        if(udiff<0)
            udiff+=1000000;
        udiff/=1000;
        diff+=udiff;
        getrusage(RUSAGE_SELF,&use);
        printf("-- Statistics --\n");
        printf("Elapsed time: %i milliseconds\n",diff);
        printf("Page Faults: %i\n",(int)use.ru_majflt);
        printf("Page Faults (reclaimed): %i\n\n\n",(int)use.ru_minflt);
    }






