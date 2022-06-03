#include "pipex.h"

//TODO: Change the input and output with dup2 somewhere

char	**get_command(char **arguments, int n)
{	
	//arguments: ./pipex infile cmd1 cmd2 outfile
	//		0      1     2    3      4
	
	char	**cmd;
	
	cmd = ft_split(arguments[n + 1], ' ');
	return (cmd);
}

int	check_str(char *haystack, char *needle)
{
	int	hay;
	int	nee;

	hay = 0;
	nee = 0;
	while (haystack[hay])
	{
		while (haystack[hay] == needle[nee] && haystack[hay] && needle[nee])
		{
			hay++;
			nee++;
			if (needle[nee] == 0)
				return (1);
		}
		nee = 0;
		hay++;
	}
	return (0);
}

char	*find_str(char **ptr, char *crit)
{
	int	i;

	i = 0;
	while(ptr[i])
	{
		if (check_str(ptr[i], crit) == 1)
			return (ptr[i]);
		i++;
	}
	return (NULL);
}

void pipex(int fd1, int fd2, char **arguments, char **env)
{
	char	* const *cmd;
	int	spoon;

	spoon = fork();
	if (spoon < 0)
		return (perror("Fork: "));
	//Child process
	if (spoon == 0)
	{
		cmd = get_command(arguments, 1);
		child(fd1, cmd, env);
	}
	//Parent process
	else
	{
		cmd = get_command(arguments, 2);
		parent(fd2, cmd, env);
	}
}

int last_arg(char **args)
{
	int	i;
	
	i = 0;
	while (args[i + 1])
		i++;
	return (i);
}

int main(int argc, char **argv, char **envp)
{
	int	fd1;
	int	fd2;

	if (argc < 5)
	{
		printf("Not enough arguments...");
		return (0);
	}
	access(argv[1], R_OK);
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd1 < 0 || fd2 < 0)
		return (-1);
	pipex(fd1, fd2, argv, envp);
}
