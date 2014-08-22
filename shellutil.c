#include "shellutil.h"

/*
 * Opens History File .ssh_history
 */
FILE * openFile()
{
	FILE * fin = NULL;
	fin = fopen(".ssh_history","a+");
	return fin;
}// End openFile

/*
 * Counts # of History Commands in History File
 * Used for finding last 200 commands in File.
 */
int findCount(FILE * fin)// Finds word count
{
	int x, count = 1;

    if (!fin) 
    	return -1;
    else
    {
	while ( (x = fgetc(fin)) != EOF)// Counts each line in file
     
     if (x == '\n' || x == '\r')// if x reaches a carriage return or new line feed
         ++count;
 	}
         return count;
}// End findCount

/*
 * Converts a given string to an integer.
 */
int stringToInt(char * s)
{
	int result = 0, negative = 1, x = 0;

			for(; x < strlen(s); x++){
				if(s[x] == '-'){
					negative = -1;
					x = 1;
				}
				result *= 10; // Add trailing zero
				result += (s[x] - '0');// Give int value
			}


			return result * negative;
}// End stringToInt

/*
 * Adds the last 200 History Commands from the History File into a Linked List.
 */
void processFile(FILE * fin, struct Node ** head, int count)
{
	char temp[MAX];
	int last = 0, x = 0;
	
	if(count > 200)
		last = count - 200;
	
	if(fin == NULL)
		openFile();
	else
	{
		rewind(fin);
		while(!feof(fin))
		{
			fgets(temp, MAX, fin);
			x++;
			if(x > last && count > 200 || count < 201)
				addOrdered(temp,head);
		}
	}
}// End processFile

/*
 * Writes to History File upon exiting shell.
 */
void writeFile(FILE * fin, struct Node * head)
{
	int writeCount = 1;

	if(head == NULL)
		return;
	else 
	{
		Node * cur = head;

		while(cur != NULL)
		{
			if(writeCount > 200)
			fprintf(fin, "%s", ((char *)cur->data));

			cur = cur->next;
			writeCount++;
		}
	}
	    fclose(fin);
	    fin = NULL;
}// End writeFile

/*
 * If a command is longer than 100 characters,
 * this removes the excess characters and returns and Invalid Command Error.
 */
void clearBuffer(void) 
{
    int ch;

    while ((ch = getchar()) != '\n') 
    {
        if (ch < 0)
            exit(EXIT_FAILURE);
    }
}// End clearBuffer

/*
 * Strips the command of any carriage return.
 */
void strip(char *s)
{
  int len;
  len = strlen(s);
  if(s[len - 2] == '\r')
    s[len -2] = '\0';

  else if(s[len - 1] == '\n')
    s[len -1] = '\0';
}// End strip

/*
 * Trims the leading and trailing white space from a string.
 */
char * trimWhiteSpace(char * word)
{
  char *end;

  while(isspace(*word))
  	word++;

  if(*word == 0)
    return word;

  end = word + strlen(word) - 1;

  while(end > word && isspace(*end)) end--;
  *(end+1) = 0;

  return word;
}// End trimWhiteSpace

/*
 * Frees the passed in allocated array.
 */
void clean(int argc, char **argv)
{
  int x;
  for(x = 0; x < argc + 1; x++)
  {
    free(argv[x]);
    argv[x] = NULL;
  }
    free(argv);
}// End clean

/*
 * Checks if the command contains valid operators.
 * Returns 1 when valid and -1 when not valid.
 */
int isValidOperator(char * s)
{
	int x, flag;
	for(x = 0; x < strlen(s); x++)
	{
		if (s[x] =='!' || s[x] == '<' || s[x] == '>' || s[x] == '|')
			flag = 0;
		
		else
			flag = -1;
	}
	if(flag == 0)
		return 0;
	else
		return -1;
}// End isValidOperator

/*
 * Checks if the !# history command contains the correct syntax.
 * Returns -1 upon invalid syntax.
 */
int isValidHistory(char * s)
{
	int x, flag;
	if(s[0] != '!')
		return -1;
	else
	{
		for(x = 1; x < strlen(s); x++)
		{
			if(isdigit(s[x]))
				flag = 0;
			else
				return -1;
		}
	}
	return flag;
}

/*
 * Checks if the command is a valid pipe command.
 * Returns -1 upon invalid command.
 */
int isValidPipe(char * s)
{
	char temp [MAX];
	strcpy(temp, s);
	char * token1, * token2, * token3, * check, * pipe = "|";
	int x, count = 0;

	for(x = 0; x < strlen(s); x++)
	{
		if(s[x] == '|')
			count++;
	}

	if((token1 = strtok(temp, pipe)) != NULL);
	else
		return -1;

	if((token2 = strtok(NULL, pipe)) != NULL);
	else
		return -1;

	if((token3 = strtok(NULL, pipe)) != NULL)
	{	
		if(token3 != NULL && count > 2)
			return -1;

		else
			return 3;
	}	
	if(token3 == NULL && count == 2)
		return -1;

	else
		return 2;
}// End isValidPipe

/*
 * Checks command if it contains legal characters.
 * Returns -1 upon invalid character.
 */
int isValidCharacter(char * s)
{
	int x, flag;
	for(x = 0; x < strlen(s); x++)
	{
	if(isalpha(s[x]) || isspace(s[x]) || isdigit(s[x]) || s[x] == '<' || s[x] == '>' || s[x] == '|' || s[x] == '!' || s[x] == '-' || s[x] == '.' || s[x] == '/' || s[x] == '_' || s[x] == '\n' || s[x] == '\r')
		flag = 0;
	else
		return -1;
	}
	return flag;
}

/*
 * Checks if the command contains valid operators.
 * Returns 1 when valid and -1 when not valid.
 */
int isValidWord(char * s)
{
	int x, flag;
	for(x = 0; x < strlen(s); x++)
	{
	if(isalpha(s[x]) || isspace(s[x]) || isdigit(s[x]) || s[x] == '!' || s[x] == '-' || s[x] == '.' || s[x] == '/' || s[x] == '_')
		flag = 0;
	else
		return -1;
	}
	return flag;
}// End isValidWord

/*
 * Checks if the first word in the command is valid.
 * Returns -1 upon invalid word.
 */
int isValidFirstWord(char * s)
{
	char temp [MAX];
	strcpy(temp, s);
	char * token;
	token = strtok(temp, " ");

	if(isValidWord(token) == 0)
		return 0;
	else 
		return -1;
}// End isValidFirstWord

/*
 * Breaks the command down into executable arguments.
 */
int makeargs(char *s, char *** argv)
{
  char temp[100];
  strcpy(temp, s);
  char * token;

  token = strtok(temp, " ");

  int x, count = 0;

    for(x = 1; token != NULL; token = strtok(NULL, " "))
    {
      count++;
    }

    if(count < 1)
      return -1;

  token = strtok(s, " ");

  *argv = (char**)calloc(count + 1, sizeof(char*));

    for(x = 0; x < count; x++)
    {
     (*argv)[x] = (char*)calloc(strlen(token)+1, sizeof(char));
     strcpy((*argv)[x], token);
     token = strtok(NULL, " ");
    }

  (*argv)[count] = '\0';

   return count;

}// end makeargs

/*
 * Fork and Execs single commands.
 */
void forkIt(char ** argv)
{
	int status, exstat, temp = 0;
	pid_t pid;

	pid = fork();

	if(pid < 0)
	{
		printf("Error: Forking Failed.\n");
		exit(-1);
	}
	if(pid == 0)
	{

		exstat = execvp(*argv, argv);

		if(exstat < 0)
		{
			printf("Error: Exec Failed\n");
			exit(-1);
		}
	}
	else waitpid(pid, &status, 0);
}// End forkIt

/*
 * Handles Output Redirection to file.
 */
int outputRedirect(char * s)
{
	int fd;
	char * filename, * token;

	if((token = strtok(s, ">")) != NULL);
		else
			return -1;
	if((filename = strtok(NULL, ">")) != NULL);
		else
			return -1;

	filename = trimWhiteSpace(filename);

	fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	
	if(fd < 0)
	{
		perror("Failed to open file");
		return -1;
	}
	else
		dup2(fd, 1);// stdout

	return fd;
}// End outputRedirect

/*
 * Handles Input Redirection from file.
 */
int inputRedirect(char * s)
{
	int fd;
	char * filename, * token;

	if((token = strtok(s, "<")) != NULL);
		else
			return -1;
	if((filename = strtok(NULL, "<")) != NULL);
		else
			return -1;

	filename = trimWhiteSpace(filename);

	fd = open(filename, O_RDONLY);

	if(fd < 0)
	{
		perror("Failed to open file");
		return -1;
	}
	else
		dup2(fd, 0);// stdin

return fd;
}// End inputRedirect

/*
 * Handles single pipe command.
 */
void pipeIt(char ** prePipe, char ** postPipe)
{
	int fd[2], exstat, status;
	pid_t pid, pid2;

	if (pipe(fd) != 0) 
	{
	   perror("Pipe Failed");
	   exit(-1);
	}

	if((pid = fork()) == -1)
	{
	   perror("Forking Failed");
	   exit(-1); 
	}

	if(pid == 0)
	{
		// Child
		close(fd[0]);
		close(1);
		dup(fd[1]);
		close(fd[1]);

		exstat = execvp(prePipe[0], prePipe);

		if(exstat < 0)
		{
			perror("Exec Failed for prePipe Command");
			exit(-1);
		}
	}// end Child

	else
	{
			if((pid2 = fork()) == -1)
			{
				perror("Double Forking Failed");
				exit(-1);
			}
			if(pid2 == 0)
			{
				// Grandchild
				close(fd[1]);
				close(0);
				dup(fd[0]);
				close(fd[0]);

			    exstat = execvp(postPipe[0], postPipe);

			if(exstat < 0)
			{
				perror("Exec Failed for postPipe Command");
				exit(-1);
			}

	}// end Grandchild

		else
		{
			// Parent
			close(fd[0]);
			close(fd[1]);

			// Wait for Child and Grandchild to finish.
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);

		}// end Parent

	}// end else

}// End pipeIt

/*
 * Handles Pipe Command containing two pipes.
 */
void doublePipeIt(char ** prePipe, char ** midPipe, char ** postPipe)
{
		int fd[2], exstat, status, x, flag = 0, stdoutCpy;
		pid_t pid, pid2, pid3;
		stdoutCpy = dup(1);
     
	if (pipe(fd) != 0) 
	{
	   perror("Pipe Failed");
	   exit(-1);
	}

	if((pid = fork()) == -1)
	{
	   perror("Forking Failed");
	   exit(-1);
	}
     
    if (pid == 0)
    { // Child

		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);

        exstat = execvp(prePipe[0], prePipe);

		if(exstat < 0)
		{
			perror("Exec Failed");
			exit(-1);
		}
    } 
    else 
    {
		if((pid2 = fork()) == -1)
		{
		   perror("Forking Failed");
		   exit(-1);
		}
         
        if (pid2 == 0)
        {// Grandchild

				close(fd[1]);
				dup2(fd[0], 0);
				close(fd[0]);

            waitpid(pid, &status, 0);

			exstat = execvp(midPipe[0], midPipe);

			if(exstat < 0)
			{
				perror("Exec Failed");
				exit(-1);
			}
        }

    	if((pid3 = fork()) == -1)
    	{
		   perror("Forking Failed");
		   exit(-1);
		}
		if(pid3 == 0)
		{
			dup2(stdoutCpy, 1);
			dup2(fd[0], 0);
			close(fd[0]);
			close(stdoutCpy);
            waitpid(pid2, &status, 0);

            exstat = execvp(postPipe[0], postPipe);

			if(exstat < 0)
			{
				perror("Exec Failed");
				exit(-1);
			}
		}
        else 
        { // Parent
            close (fd[0]);
            close (fd[1]);

            wait(&status);
        }
    }

}// End doublePipeIt