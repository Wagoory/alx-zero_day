#include "main.h"
/**
 * _atoi_ - convert the char to int
 * @str: the string we want to convert it
 * Return: the converted int
 */
int _atoi_(char *str)
{
	int result = 0;
	int i, j;

	j = _strlen(str);
	for (i = 0; i < j; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			break;
		result = result * 10 + str[i] - '0';
	}
	return (result);
}

/**
 * check_built - check if the user input is built in function
 * @command: the input of user
 * Return: 1 if true and 0 if false
 */

int check_built(char *command)
{
	char *built_fun[] = {"exit", "env", "cd", NULL};
	int i;

	for (i = 0; built_fun[i]; i++)
	{
		if (_strcmp(command, built_fun[i]) == 0)
			return (1);
	}

	return (0);
}

/**
 * handle_built - handle the built in function
 * @args: the array of spilted command
 * @status: the status of operation
 * @command: the user input
 * @ind: the index of process
 * @argv: - the argument of main function
 */

void handle_built(char **args, int status, char *command, int ind, char **argv)
{
	int i = 0;
	char *id = NULL;

	if (_strcmp(args[0], "exit") == 0)
	{
		if (args[1])
			status = _atoi_(args[1]);
		else
		{
			_free(command, args);
			exit(status);
		}
		if (args[1][0] == '-')
		{
			id = _itoa(ind);
			write(STDERR_FILENO, argv[0], _strlen(argv[0]));
			write(STDERR_FILENO, ": ", 2);
			write(STDERR_FILENO, id, _strlen(id));
			write(STDERR_FILENO, ": exit", 6);
			write(STDERR_FILENO, ": ", 2);
			write(STDERR_FILENO, "Illegal number: ", 16);
			write(STDERR_FILENO, args[1], _strlen(args[1]));
			write(STDERR_FILENO, "\n", 1);
			_free(command, args);
			free(id);
			exit(2);

		}
		_free(command, args);
		exit(status);
	}
	else if (_strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
		{
			write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		_free(command, args);
	}
}
/**
 * _free - free two element
 * @f1: first element
 * @f2: second elment
 */
void _free(char *f1, char **f2)
{
	free(f1);
	free(f2);
}

/**
 * comment - handles the comments from the user
 * @line: the input from user
 */

void comment(char *line)
{
	int j = 0;

	if (line[j] == '#')
		(line[j]) = '\0';
	while (line[j] != '\0')
	{
		if (line[j] == '#' && line[j - 1] == ' ')
			break;
		j++;
	}
	line[j] = '\0';
}

/**
 * is_empty - check if the user give empty input
 * @str: the input of the user
 * Return: if true return 1 else return 0
 */
int is_empty(const char *str)
{
	if (str == NULL)
		return (1);

	while (*str)
	{
		if (!_isspace(*str))
			return (0);
		str++;
	}

	return (1);
}
/**
 * _isspace - check the character of input
 * @c: the character of input
 * Return: 0 if false, 1 if true
 */

int _isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r');
}

/**
 * exec - executes the user command
 * @args: the array of the splited command
 * @line: the input of the user
 * @name: shell name
 * @ind: index
 * Return: the status of the operation
 */

int exec(char **args, char *line, int ind, char **name)
{
	int status = 0;
	char *path = NULL;
	char *command;
	pid_t id = fork();

	if (id == 0)
	{
		if (_strchr(args[0], '/') == NULL)
		{
			path = _getenv("PATH");
			command = get_full_path(args[0], path);
			if (command == NULL)
			{
				print_error(name[0], args[0], ind);
				return (127);
			}
		}
		else
			command = args[0];

		if (execve(command, args, environ) == -1)
		{
			/*perror(args[0]);*/
			free(args);
			free(line);
			exit(127);
		}
	}
	else
		waitpid(id, &status, 0);
	return (WEXITSTATUS(status));
}

/**
 * _getenv - gets environment path
 * @viron: the env variable
 * Return: the path
 */
char *_getenv(char *viron)
{
	size_t len = _strlen(viron);
	char **env = environ;

	while (*env)
	{
		if (_strncmp(*env, viron, len) == 0 && (*env)[len] == '=')
			return (&(*env)[len + 1]);
		env++;
	}
	return (NULL);
}

/**
 * _getline - read the input
 * @lineptr: the place to store the data
 * @m: buffer size
 * @stream: the file to read the line from
 * Return: 0 in success and -1 in fail
 */
size_t _getline(char **lineptr, size_t *m, FILE *stream)
{
	size_t li = 0;
	int i;
	char c;

	if (lineptr == NULL || m == NULL || stream == NULL)
		return (-1);

	if (*lineptr == NULL)
	{
		*m = 15024;
		*lineptr = malloc(*m);

		if (*lineptr == NULL)
			return (-1);
	}

	while ((i = read(fileno(stream), &c, 1)) == 1)
	{
		if (c == '\n')
			break;

		(*lineptr)[li++] = c;
	}

	if (i == 0)
		return (-1);

	(*lineptr)[li] = '\0';
	return (li);
}

/**
 * get_full_path - gets the path of the user command
 * @command: the user input
 * @path: the environment path
 * Return: the path
 */
char *get_full_path(char *command, char *path)
{
	char *token;
	char *full_path = NULL;

	token = _strtok(path, ":");
	while (token != NULL)
	{
		full_path = malloc(_strlen(token) + _strlen(command) + 1);
		if (full_path == NULL)
			return (NULL);

		_strcpy(full_path, token);
		_strcat(full_path, "/");
		_strcat(full_path, command);

		if (access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		token = _strtok(NULL, ":");
	}

	return (NULL);
}

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
	return (write(1, &c, 1));
}

/**
 * _puts - function print string
 * @str: input value
 */

void _puts(char *str)
{
	while (*str != '\0')
	{
		_putchar(*str + 0);
		str++;
	}
	_putchar('\n');
}

/**
 * print_error - displays error message
 * @name: shell name
 * @command: user command
 * @ind: error index
 */
void print_error(char *name, char *command, int ind)
{
	char *id, msg[] = ": not found\n";

	id = _itoa(ind);
	write(STDERR_FILENO, name, _strlen(name));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, id, _strlen(id));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, command, _strlen(command));
	/*write(STDERR_FILENO, ": ", 2);*/
	write(STDERR_FILENO, msg, _strlen(msg));
	free(id);
}

/**
 * _itoa - change the int to char
 * @n:the int number
 * Return: the char
 */
char *_itoa(int n)
{
	char buff[25];
	int i = 0;

	if (n == 0)
		buff[i++] = '0';
	else
	{
		while (n > 0)
		{
			buff[i++] = (n % 10) + '0';
			n /= 10;
		}
	}
	buff[i] = '\0';
	rev_str(buff, i);

	return (_strdup(buff));
}

/**
 * rev_str - reverse string
 * @str: string to be reverted
 * @len: string length
 */
void rev_str(char *str, int len)
{
	int begin = 0, end = len - 1;
	char copy;

	while (begin < end)
	{
		copy = str[begin];
		str[begin] = str[end];
		str[end] = copy;
		begin++;
		end--;
	}
}

/**
 * read_line - read the line from user
 *
 * Return: return the line from user
 */

char *read_line(void)
{
	char *new_line = NULL;
	size_t buff_len = 0;
	ssize_t chars;

	if (isatty(STDIN_FILENO))
		write(1, "$ ", 2);

	chars = _getline(&new_line, &buff_len, stdin);

	if (chars == -1)
	{
		free(new_line);
		return (NULL);
	}
	return (new_line);
}

/**
 * spilt_line - splits the command entered from the user
 * @new_line: the input from user
 * Return: the array of the splited command
 */

char **spilt_line(char *new_line, char *del)
{
	int len = 0;
	char **args = NULL;
	char *token = NULL;

	args = malloc(sizeof(char *) * 25);
	if (args == NULL)
		return (NULL);
	token = _strtok(new_line, del);
	if (!token)
	{
		free(new_line);
		return (NULL);
	}
	while (token != NULL)
	{
		args[len] = token;
		len++;
		token = _strtok(NULL, del);
	}
	args[len] = NULL;
	return (args);
}

/**
 *  _strlen - print length of string
 *  @str: the string
 *
 *  Return: length
 */
int _strlen(char *str)
{
	int len = 0, i;

	for (i = 0; str[i] != '\0'; i++)
		len++;
	return (len);
}

/**
 * _strcat - concatenates 2 strings
 * @dest: string to be appended to
 * @src: string to append
 *
 * Return: concatenated string
 */
char *_strcat(char *dest, char *src)
{
	int i = 0, j;

	while (dest[i] != '\0')
	{
		i++;
	}
	for (j = 0; src[j] != '\0'; j++)
	{
		dest[i + j] = src[j];
	}
	dest[i + j] = '\0';
	return (dest);
}

/**
 * *_strcpy - copies the string pointed to by src
 * including the terminating null byte (\0)
 * to the buffer pointed to by dest
 * @dest: pointer to the buffer in which we copy the string
 * @src: string to be copied
 *
 * Return: return the copied string
 */
char *_strcpy(char *dest, char *src)
{
	int length, i;

	length = 0;

	while (src[length] != '\0')
	{
		length++;
	}

	for (i = 0; i < length; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return (dest);
}

/**
 * _strcmp - compares 2 strings
 * @s1: first string
 * @s2: second string
 *
 * Return: 0 if it the same
 */

int _strcmp(char *s1, char *s2)
{
	int i = 0;

	while (s1[i] - s2[i] == 0 && s1[i] != '\0')
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

/**
 * _strchr - Returns a pointer to the first
 *       occurrence of the character
 * @s: string to search
 * @c: character to be searched for
 *
 * Return: the location
 */

char *_strchr(char *s, char c)
{
	int i;

	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] == c)
		{
			return (&s[i]);
		}
	}
	return (0);
}

/**
 * *_strdup - copy of the string
 * @str: input
 * Return: pointer
 */

char *_strdup(char *str)
{
	int i, size;
	char *m;

	if (str == NULL)
		return (NULL);

	for (size = 0; str[size] != '\0'; size++)
		;
	size++;
	m = malloc(size * sizeof(*str));

	if (m == 0)
		return (NULL);

	for (i = 0; i < size; i++)
		m[i] = str[i];
	return (m);
}

/**
 * _strncmp - compare two strings
 * @str1: first string
 * @str2: second string
 * @num: size
 * Return: comparsion
 */
int _strncmp(const char *str1, const char *str2, size_t num)
{
	size_t i;

	for (i = 0; i < num; i++)
	{
		if (str1[i] == '\0' || str1[i] != str2[i])
		{
			return (str1[i] - str2[i]);
		}
	}

	return (0);
}

/**
 * _strtok - breaks the string into tokens
 * @str: string to be breaked down
 * @delim: separator
 * Return: poiner to the next token
 */
char *_strtok (char *str, char *del)
{
	static char *token;
	char *tokenizer;

	if (str != NULL)
		token = str;

	if (token == NULL)
		return (NULL);

	while (*token != '\0' && _strchr(del, *token) != NULL)
		token++;

	if (*token == '\0')
		return (NULL);

	tokenizer = token;

	while (*token != '\0' && _strchr(del, *token) == NULL)
		token++;

	if (*token != '\0')
	{
		*token = '\0';
		token++;
	}

	return (tokenizer);
}
