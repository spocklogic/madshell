#include "shell.h"

void prompt() {
	/* Prints a suitable prompt for use in the shell.
	 * 	*/
	char *home = getenv("HOME");
	char *cwd = getenv("PWD");
	char *hostname = (char *)malloc(256);
	gethostname(hostname, 256);
	char *username = getenv("USER");
	printf("\e[31;1m\e[40;1m(MAD)\e[0m");
	printf("%s@%s:", username, hostname);
	if(strstr(cwd, home)) {
		printf("~%s", cwd+strlen(home));
	}
	else {
		printf("%s", cwd);
	}
	printf("$ ");
	fflush(stdout);
}

char*** inputLine() {
	/** Responsible for user interaction.
	 * 
	 * 	Returns:
	 * 		Input to be executed, divided up into commands and their options.
	 */
	char s[256];
	char*** scol = (char ***)malloc(sizeof(char **) * 256);
	//char (**scol)[256] = malloc(sizeof(char[256][256][256]));
	//http://stackoverflow.com/questions/30117161/why-do-i-need-to-use-type-to-point-to-type/30117625#30117625
	prompt();
	fgets(s, sizeof(s), stdin);
	char* news;
	char* sp = s;
	sp = strsep(&sp, "\n");
	int c = 0;
	while(sp) {
		news = strsep(&sp, ";");
		scol[c] = inputCommand(news);
		//printf("%s\n", scol[0][1]);
		c++;
	}
	scol[c] = 0;
	printf("%s\n", scol[0][0]);
	printf("%s\n", scol[0][1]);
	return scol;
}

char** inputCommand(char* input) {
	char** sar = (char **)malloc(sizeof(char *) * 256);
	char* news;
	int c = 0;
	while(input) {
		news = strsep(&input, " ");
		sar[c] = news;
		c++;
	}
	sar[c] = 0;
	return sar;
}

int execute(char** command) {
	pid_t chP = fork();
	int status = 0;
	if(chP == 0) {
		int c = execvp(command[0], command);
		exit(errno);
	}
	else {
		wait(&status);
	}
	return WEXITSTATUS(status);
}

