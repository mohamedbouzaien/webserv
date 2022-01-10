#include "header.hpp"
#include <list>

int main(int ac, char **av) {
	int pid;
	char **s ;
	s = (char **)malloc(sizeof(char *) * 2);
	s[0] = "/usr/local/Cellar/php/8.1.1/bin/php-cgi";
	s[1] = NULL;

	char **env;
	env = (char **)malloc(sizeof(char *) * 5);

	env[0] = "SERVER_PROTOCOL=HTTP/1.1";
	env[1] = "SCRIPT_FILENAME=/Users/adriencastelbou/Desktop/42_sandbox/client-server-test/test.php";
	env[2] = "SCRIPT_NAME=test.php";
	env[3] = "REDIRECT_STATUS=200";
	pid = fork();
	if (pid == 0) // Child
	{
		execve(s[0], s, env);
		exit(1);
	}
	else
		wait(NULL);
	free(s);
	free(env);
}
