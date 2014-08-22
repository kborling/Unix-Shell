#include "cscd340s14hw3.h"

int main() {

	char s[LIMIT], ** argv, ** pargv1, ** pargv2, ** pargv3, * out = ">", * in = "<", * pipe = "|", * dir = NULL, * incheck = NULL, * outcheck = NULL, * pipecheck = NULL, * temp = NULL, * token = NULL, * token1 = NULL, * token2 = NULL, * token3 = NULL, * user = NULL, cwd [1024];
	int argc = 0, pargc1 = 0, pargc2 = 0, pargc3 = 0, stdinCpy = dup(0), stdoutCpy = dup(1), fd = -1, fd2 = -1, count = -1, num = 0, flag = 0;
	FILE * fin = NULL;
	Node * head = NULL;

	// Get the current username
	user = getenv("LOGNAME");

	// Load History into linked list
	fin = openFile();
	count = findCount(fin);
	processFile(fin, &head, count);

	// Print Username and current working directory to screen
    getcwd(cwd, sizeof(cwd));
    printf("%s:%s$ ", user, cwd);
    fgets(s,LIMIT, stdin);

	while(strcmp(s, "exit\n") != 0)
	{
		flag = 0;
		// Add command to history
		addOrdered(s, &head);
		// Handle just carriage return
		while (s[0] == '\n' || s[0] == '\r')
		{
			getcwd(cwd, sizeof(cwd));
			printf("%s:%s$ ", user, cwd);
			fgets(s,LIMIT, stdin);
			if(strcmp(s, "exit\n") == 0)
				exit(0);
		}
		// Handle input containing invalid first word
		while(isValidFirstWord(s) == -1)
		{
			getcwd(cwd, sizeof(cwd));
			printf("Error: Invalid Command\n%s:%s$ ", user, cwd);
      		fgets(s,LIMIT, stdin);
      		isValidFirstWord(s);
      		if(strcmp(s, "exit\n") == 0)
				exit(0);
		}
		// Handle input containing any invalid characters
		while (isValidCharacter(s) == -1)
		{
			getcwd(cwd, sizeof(cwd));
			printf("Error: Invalid Command\n%s:%s$ ", user, cwd);
      		fgets(s,LIMIT, stdin);
      		isValidCharacter(s);
      		if(strcmp(s, "exit\n") == 0)
				exit(0);
		}
		// Handle input larger than 100 characters
		while (NULL == strchr(s, '\n'))
		{
			getcwd(cwd, sizeof(cwd));
			printf("Error: Invalid Command\n%s:%s$ ", user, cwd);
            clearBuffer();
      		fgets(s,LIMIT, stdin);
      		if(strcmp(s, "exit\n") == 0)
				exit(0);
        }
        // Handle history command
        if(strcmp(s, "history\n") == 0)
		{
			printList(head);
		}
		// Handle !! comand
		if(strcmp(s, "!!\n") == 0)
		{
			temp = (char *)findLast(head);
			strcpy(s, temp);
		}

		// Remove carriage return
		strip(s);
		// Handle history command !##
		if(isValidHistory(s) == 0)
		{
			token = strtok(s, "!");
			num = stringToInt(token);
			temp = (char *)findItem((void*)num, head);

			if(temp == NULL)
			{
				getcwd(cwd, sizeof(cwd));
				printf("%s: event not found\n%s:%s$ ", s, user, cwd);
				fgets(s,LIMIT, stdin);
			}
			else
				strcpy(s, temp);
			strip(s);
		}

		// Check for input redirection
		incheck = strchr(s, *out);
		// Check for output redirection
		outcheck = strchr(s, *in);
		// Check for pipe command
		pipecheck = strchr(s, *pipe);

		// Handle output redirection
		if(incheck && !pipecheck)
		{
			fd = outputRedirect(s);

			if(fd == -1)
			{
				printf("Error: Invalid Command\n");
				flag = 1;
			}
		}
		// Handle input redirection
		if(outcheck && !pipecheck)
		{
			fd2 = inputRedirect(s);
			
			if(fd2 == -1)
			{
				printf("Error: Invalid Command\n");
				flag = 1;
			}
		}
		
		// Handle Redirect with Pipe and displays error
		if((pipecheck && outcheck) || (pipecheck && incheck))
		{
			flag = 0;
			printf("Error: Invalid Command\n");
			getcwd(cwd, sizeof(cwd));
			printf("%s:%s$ ", user, cwd);
			fgets(s, LIMIT, stdin);
			if(strcmp(s, "exit\n") == 0)
				exit(0);
			strip(s);
		}
		
		// Handles Pipe Commands while not allowing redirects
		if(pipecheck && flag == 0 && !outcheck && !incheck)
		{
			int result = isValidPipe(s);
			// Handle missing token after pipe
			
			if(result == -1)
				printf("Error: Invalid Command\n");
			// Handle Pipe Command
			if(result == 2)
			{
				token1 = strtok(s, "|");
				token2 = strtok(NULL, "|");
				trimWhiteSpace(token1);
				trimWhiteSpace(token2);
				pargc1 = makeargs(token1, &pargv1);
				pargc2 = makeargs(token2, &pargv2);
				pipeIt(pargv1, pargv2);
				clean(pargc1, pargv1);
				clean(pargc2, pargv2);
			}
			// Handle Double Pipe Command
			if(result == 3)
			{
				token1 = strtok(s, "|");
				token2 = strtok(NULL, "|");
				token3 = strtok(NULL, "|");
				trimWhiteSpace(token1);
				trimWhiteSpace(token2);
				trimWhiteSpace(token3);
				pargc1 = makeargs(token1, &pargv1);
				pargc2 = makeargs(token2, &pargv2);
				pargc3 = makeargs(token3, &pargv3);
				doublePipeIt(pargv1, pargv2, pargv3);
				clean(pargc1, pargv1);
				clean(pargc2, pargv2);
				clean(pargc3, pargv3);
			}

			flag = 1;
		}
		// Handles Directory Change Command
		if(strstr(s, "cd") != NULL)
		{
			dir = strtok(s, " ");
			dir = strtok(NULL, " ");
			trimWhiteSpace(dir);
			chdir(dir);
			flag = 1;
		}
		// Handles making arguments for any single command
		if(flag == 0)
	    	argc = makeargs(s, &argv);
		
	    // Handles Executing and Cleaning any single command
	    if (argc > 0 && flag == 0)
	    {
		    //printargs(argc, argv);// REMOVE
		    if(strcmp(s, "history") != 0)
		    	forkIt(argv);
		    clean(argc, argv);
		}
		// Handles closing stdout
		if(fd > 0)
	    {
		    close(fd);
			dup2(stdoutCpy, 1);
		}
		// Handles closing stdin
		if(fd2 > 0)
		{
			close(fd2);
			dup2(stdinCpy, 0);
		}
		// Prints error if makeargs returns error
		if (argc == -1)
	    	printf("Error: Invalid Command\n");

	    // Reprint username and current working directory
	    getcwd(cwd, sizeof(cwd));
		printf("%s:%s$ ", user, cwd);
		fgets(s, LIMIT, stdin);
		}// End while

		// Writes to History file
		writeFile(fin, head);
		// Clears data from History Linked List
		if(head != NULL)
			clearList(head);

	return 0;
}// End main