#include "main.h"

int separator(char **tokens, char *line, int ind, char **argv)
{
	char *del = " ; ";
	char *semicolon = strtok(line, del);
	char *split_cmd;
	int status = 0;

	while (semicolon != NULL)
	{
		split_cmd = semicolon;
		while (*split_cmd && (*split_cmd == ' ' || *split_cmd == '\t'))
			split_cmd++;

		if (*split_cmd)
		{
			status = exec(tokens, semicolon, ind, argv);
		}

		semicolon = strtok(NULL, del);
	}
	return (status);
}
