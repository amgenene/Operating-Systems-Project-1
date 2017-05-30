#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_INPUT (120 )

struct timeval printStats(struct timeval start);
struct pInfo{
int pid;
struct timeval start;
int queue;
struct pInfo *next;
};
int main(void){

    struct timeval start;
    char option;
    char *line=NULL;
    char *argus=NULL;
    char *path=NULL;
    char *path2=NULL;
    size_t len = 0;
    int procNum =1;//order of background processess
    int run = 1;//flag to tell if running process command is running 1=off 0=on
    ssize_t read;
    char command[80];
    int numRead;//num reader
    struct pInfo *root;//root            
    char cmd[MAX_INPUT][128];
    int counter = 2;
    while(1){
        printf("===== Mid-Day Commander, v0 =====\n");
        printf("G’day, Commander! What command would you like to run?\n ");
        printf("\t0. whoami: Prints out the result of the whoami command\n");
        printf("\t1. last  : Prints out the result of the last command\n");
        printf("\t2. ls    : Prints out the result of a listing on a user-specified path\n");
        if(counter > 2)
        	for(int i = 3; i <= counter; i++)
        		printf("\t%d. %s: user added command\n", i, cmd[i-1]);
        printf("\ta. add command   : Adds new a new command to the menu\n");
        printf("\tc. change directory: Changes process working directory\n");
        printf("\te. exit   : leave midday commander\n");
        printf("\tp. pwd : prints working directory\n");
	 if(run)	 
			printf("\tr. running processes : prints list of running processes\n");
        printf("Option?: ");
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
        else if(line[0]=='e'||line[0]==EOF){
            printf("Logging you out Commmander.\n");
            exit(1);
        }
        else if(line[0]=='a'){
            printf("--Add a command--\n");
            printf("Command to add?: ");
			while(numRead=getline(&path2,&len,stdin)==-1);
			if(numRead>128)
			{
				printf("Invalid Input. Must have less than 128 characters");
				break;
			}
	    printf("here?");
			strtok(path2,"\n");
            strncpy(cmd[counter], path2,100);
	    printf("here12");
            counter++;
			printf("Okay, added with ID %i!",counter);  
        }   
        else if(line[0]=='p'){
            char cwd[1024];
               if (getcwd(cwd, sizeof(cwd)) != NULL)
                       fprintf(stdout, "--Current Directory--\n Directory: %s\n", cwd);
               else
                       perror("getcwd() error");
        }
        else if(line[0]=='c'){
            printf("-- Change Directory --\n");
            printf("New Directory?: ");
            char dir[128];
            scanf("%s", dir);
            chdir(dir);
            char c;
            while(c = getchar()!='\n'&& c != EOF);
        }
        else
        {
	                
			int flag=1;
			for(int i=3;i<=counter;i++)
				if(line[0]==(i+'0'))
				{
				  printf("here");
        			printf("-- %s --",cmd[i-1]);
					char *save3;
					char * copy = (char *)malloc(strlen(cmd[i-1]));//creates a copy
					printf("here1");
					int num=0;
					char * args2[128];
					strcpy(copy,cmd[i-1]);
        			if((args2[num]=strtok_r(copy," \n\r\t",&save3))!=NULL)
        			{
				  printf("here2");
					    num++;       
					    while((args2[num]=strtok_r(NULL," \n\r\t",&save3))!=NULL)
					        num++;
					}
					if(num>32)
					{
					  printf("here3");
						printf("Invalid Input. Must have less than 32 Arguments");
						break;
					}
					if(args2[num][0]=='&')
					{
					  printf("here4");
						struct pInfo process={1,NULL,1,NULL};//starts out as null 						
						num--;//removes ampersand from arguments
						process.queue=procNum;//set queue to process num
						process.pid=fork();//fork the pid
						if(process.pid < 0) {
							printf("fork failed\n");
							exit(1);   
						}
						else if(process.pid == 0) {
							gettimeofday(&process.start,NULL);
							char *myargs2[num+1];
							for(int j=0;j<num;j++) { 
							  myargs2[j]=args2[j];
							}
							myargs2[num]=NULL;
							printf("\n");
							printf("here1");
							execvp(myargs2[0],myargs2);
							printf("\n");
						}
						else
						{
							int child;							
							child=wait3(0,WNOHANG,NULL);//wait for any proccess to finish
							if(child>0)
								if(process.pid==child)// child is still there and process.pid =child
									printStats(process.start);//print the stats from the start
						}
							
					}
					else{
						int rc=fork();		
						if(rc < 0) {
							printf("fork failed\n");
							exit(1);   
						}
						else if(rc == 0) {
							gettimeofday(&start,NULL);
							char *myargs2[num+1];
							for(int j=0;j<num;j++)
							    myargs2[j]=args2[j];
							myargs2[num]=NULL;
							printf("\n");
							execvp(myargs2[0],myargs2);
							printf("\n");
						}
						else
							wait3(0,0,NULL);
					}
					flag=0;
					free(copy);
				}
			if(flag)
           		printf("Invalid Input. Please try again.");
		}
        printf("\n\n");
        printStats(start);	
    }
    free(argus);
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

