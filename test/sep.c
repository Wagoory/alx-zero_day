#include "main.h"

int separator(char *new_line, char **tokens, char *line, int ind, char **argv)
{
	char *del = " ; ";
	char *semicolon = strtok(new_line, del);
	char *split_cmd;
	int status = 0;

	while (semicolon != NULL)
	{
		split_cmd = semicolon;
		while (*split_cmd && (*split_cmd == ' ' || *split_cmd == '\t'))
			split_cmd++;

		if (*split_cmd) {
			status = exec(tokens, line, ind, argv);
		}

		semicolon = strtok(NULL, del);
	}
	return (status);
}
