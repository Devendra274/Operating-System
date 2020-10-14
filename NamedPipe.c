/*
	4. Write a program to Implement Named Pipe. 
     a) Pass a Message(sentence) from Parent to Child.
     b) Child should Print the Message on Terminal.
     c) Use exec to Create Directory ‘data’ at Current Location After Receiving the Message.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>

int READ=0, WRITE=1; 

int main(int argc, char *argv[])
{
	char* namedPipe = "./namedPipeFile";
	int ret = mkfifo(namedPipe, S_IRUSR| S_IWUSR);
	
	if(ret == -1)																		//Error Prompt: In case Of Pipe Creation Failure
	{
		printf("Pipe Creation ERROR\n");				
		return -1;
	}

	char* msgString; 
	msgString = (char*)malloc(1000);								//Dynamic Memory Allocation For Message
	
	if(msgString == NULL) 
	{ 
  	printf("Memory not allocated.\n"); 
  	exit(0); 
  }
	
	printf("Parent Process----------------\n");
	printf("You can enter maximum 1000 Character Length of Message\n");
	fgets(msgString, 1000, stdin);
	
	pid_t pid = fork(); 														//Child Process Creation
	
	if(pid < 0)																			//If Forking Failed Then Terminate with Error.
	{
		printf("Child Not Created- FORKING ERROR\n");
		return -1;
	}
	
	if(pid == 0)																		//Success Fork
	{
		printf("Child Process------------\n");
		char* receiver;
		receiver= (char*)malloc(sizeof(msgString)); 	//Dynamic Memory for storing the message send my Parent via pipe
		int disp= open(namedPipe, O_RDONLY);					//Child set with Read perm.
		read(disp, receiver, strlen(msgString));			//Caught the msg send by Parent
		printf("%s",receiver);
		close(disp);																	//Close Pipe Connection
	}
	else																						//Parent here
	{
		int disp= open(namedPipe, O_WRONLY);					//Parent set with Write perm.
		write(disp, msgString, strlen(msgString)+1);
		waitpid(pid, NULL, 0);												//Waiting for child Execution
		close(disp);																	//Close Pipe Connection
		printf("Creating Data dir at pwd....");
		char *args[]={"/bin/mkdir" , "./Data",NULL};
    execv(args[0],args);													//Creation Of Directory
	}

	if(remove("namedPipeFile") == 0)
	{
		printf("NamedPipe Removed\n");
	}

	return 0;
}

