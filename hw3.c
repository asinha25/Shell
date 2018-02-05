/*
>
> Name: Aditya Sinha
> Lab 3: Writing your own shell
> Description: This is a simple program that will act as a shell - see program details
> NetID: asinha

=============================
====== Program Details ======
=============================
> Displays a command prompt and reads in commands from the user. 
> Parses the command line into arguements, creating an array of character pointers. 
> Forks off a child and has the child load the requested program - parent reports the PID of the child. 
> Prints the exit value once the child has completed executing and reports the exit value. 
> Supported commands:
> 	* exit 				  - Exits out the shell
> 	* command > filename  - Redirects the output of the command to the filename (overwrites existing contents)
> 	* command >> filename - Redirects the output and appends to the contents of the filename (no overwrite)
> 	* command < filename  - Command reads its input from filename instead from stdin.
>   * Ctrl-C 			  - Terminates a running program, doesn't exit. Prints "SIGNINT handled".
> 	* Ctrl-Z 			  - Sends a stop signal, doesn't exit. Prints "SIGTSTP handled".
>
> Not supported:
>	* Background process
> 	* Multiple child processes
>
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>

void sigint_handler(int sig)
{
	char message[] = "\nSIGINT handled.\n361>";
	write (1, message, sizeof(message));
}

void sigtstp_handler(int sig)
{
	char message[] = "\nSIGTSTP handled.\n361>";
	write (1, message, sizeof(message));
}

int main()
{

	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);

	char line[500];		// create space for array - assume less than 500 chars
	char *args[20]; 	// create space for arguements  
	
	while(1)
	{
		printf("361> ");				// print prompt
		fgets(line, 500, stdin);		// read line from terminal
		line[strcspn(line, "\n")] = 0;	// remove newline character

		// if user input = exit, exit out
		if(strcmp (line, "exit") == 0)
		{
			exit(0);
		}
		else
		{
			printf("");
		}
		
		// break string into words and store into array
		char *word = strtok(line, " ");
		char *filename; 
		int i = 0;
		int redirection;
		int status;
		int counter = 0;

		while(word)
		{
			if(strcmp(word, ">") == 0 || strcmp(word, ">>") == 0 || strcmp(word, "<") == 0) 
			{
				if(strcmp(word, ">") == 0) 
				{
					redirection = 1;
				}
				else if(strcmp(word, ">>") == 0)
				{
					redirection = 2;
				}
				else if(strcmp(word, "<") == 0)
				{
					redirection = 3;
				}

				word = strtok(NULL, " ");
				filename = (char *)malloc(sizeof(char) * 100);
				strcpy(filename, word);
				word = strtok(NULL, " ");
			}
			else
			{
				args[i] = (char *)malloc(sizeof(char) * 100);
				strcpy(args[i], word);			// copy a word to the arg array
				word = strtok(NULL, " ");		// get next word
				i++;
				counter++;
			}
	}

	args[i] = malloc(100*sizeof(char));
	args[i] = NULL;
	
	int exit_status = 0;

	// forking off a child process
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Failed to fork process!\n");
		exit(-1);
	}
	if(pid == 0)
	{
		printf("PID: %d\n", getpid());
		int file;

		// existing contents are overwritten
		if(redirection == 1)
		{
			file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IROTH);
			dup2(file, 1);
		}

		// output is appended to the contents of filename
		else if(redirection == 2)
		{
			file = open(filename, O_APPEND | O_CREAT | O_WRONLY, S_IROTH);
			dup2(file, 1);
		}

		// reads input from filename instead of stdout
		else if(redirection == 3)
		{
			file = open(filename, O_RDONLY, S_IROTH);
			dup2(file, 0);
		}

		execvp(args[0], args);
	}
	else
	{
		wait(NULL);
		printf("Exit: %d\n", exit_status);
	}

	for(i = 0; i < counter; i++)
	{
		free(args[i]);
	}	
}

	// end while loop
	return 0;
}	
