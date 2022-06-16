#include "pipex.h"

char	*ft_join(char *s1, char *s2)
{
	/*
	 * malloc new string with size of s1 + s2 + 1 (NULL terminated)
	 * if s2 is null, return copy of s1
	 * if s1 is null, return copy of s2
	 * iterate through each string and copy each character
	 * null terminate
	 * return
	 */
	
	int	i;
	int	j;
	char	*joined_str;

	joined_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!joined_str)
		return (0);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		joined_str[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2 && s2[i])
	{
		joined_str[j] = s2[i];
		i++;
		j++;
	}
	joined_str[j] = 0;
	return (joined_str);
}

char	**ft_freeall(char **ptr, int i)
{
	while(i)
		free(ptr[--i]);
	free(ptr);
	return (0);
}

char	**ft_joinall(char **ptr, char *s2)
{
	int	i;
	char	**final;

	i = 0;
	while(ptr[i])
		i++;
	final = malloc(sizeof(char *) * i + 1);
	if (!final)
		return (0);
	i = 0;
	while(ptr[i])
	{
		final[i] = malloc(sizeof(char) * ft_strlen(ptr[i]) + 1);
		if (!final[i])
			return (ft_freeall(final, i));
		final[i] = ft_join(ptr[i], s2);
		i++;
	}
	return (final);
}

char	*string_finder(char **ptr, char *str)
{
	/*
	 * iterate through double pointer
	 * if start of string is identical to str, exit loop
	 * malloc new str with same length + 1 (NULL TERMINATION)
	 * copy one by one each character
	 * null terminate
	 * return
	 */

	int	i;
	int	j;

	i = 0;
	j = 0;
	printf("\nstring_finder\n-------------\n");
	while (ptr[i])
	{
		while (ptr[i][j] == str[j])
		{
			printf("ptr[%d][%d]: %c == str[%d]: %c\n", i, j, ptr[i][j], j, str[j]);
			j++;
		}
		if (!str[j])
		{
			printf("should make it here...\n");
			while(ptr[i][j])
				j++;
			ptr[i][j] = 0;
			printf("ptr[i]: \"%s\"\n", ptr[i]);
			return (ptr[i]);
		}
		else
			j = 0;
		i++;
	}
	return (0);
}

void	child(t_pipex pipex, char *cmd)
{
	/*
	 * dup2 to make STDIN the infile
	 * dup2 to make the STDOUT the pipe
	 *
	 * close read side of pipe (we're not reading from the pipe)
	 * split the command passed
	 * [ls] [-l] [-a] [\0]
	 *
	 * Using "access", check if path + command exists and is executable
	 * Check for all paths
	 *
	 * /usr/local/sbin/ls - /usr/local/bin/ls - /usr/bin - /usr/lib/jvm/default/bin/ls...
	 *
	 * execve(THE PATH, EXTRA ARGUMENTS, ENVIRONMENT)
	 * example:
	 * execve(cmd_path[i], split_cmd, pipex.environment);
	 * if all fails, error
	 * close that end of the pipe
	 */
	int	i;
	int	err;
	char	*temp_path;

	i = 0;
	dup2(pipex.fd1, 1);
	dup2(pipex.tube[1], 0);

	close(pipex.tube[0]);
	pipex.split_cmd = ft_split(cmd, ' ');
	while (pipex.all_paths[i])
	{		
		temp_path = ft_join(pipex.all_paths[i], pipex.split_cmd[0]);
		if(access(temp_path, F_OK | X_OK) == 0)
		{
			err = execve(temp_path, &pipex.split_cmd[1], pipex.environment);
			if (err == -1)
			{
				free(temp_path);
				perror("child execve");
			}
		}
		i++;
	}
	close(pipex.tube[1]);

}

void	parent(t_pipex pipex, char *cmd)
{
	/*
	 * dup2 to make STDIN the pipe
	 * dup2 to make the STDOUT the outfile
	 *
	 * close write side of the pipe (we're not reading from the pipe)
	 * split the command passed
	 * [ls] [-l] [-a] [\0]
	 *
	 * using "access" check if each path + command exists and is executable
	 * check for all paths
	 *
	 * /usr/local/sbin/ls - /usr/local/bin/ls...
	 *
	 * waitpid for child process (no idea how to do that lmao)
	 * probably before trying to do anything, might even need to wait at the very beginning
	 * we'll see...
	 *
	 * execve(THE PATH, EXTRA ARGUMENTS, ENVIRONMENT)
	 * example:
	 * execve(cmd_path[i], split_cmd, pipex.environment);
	 * if all fails, error
	 * close end of the pipe
	 */
	int	i;
	int	err;
	char	*temp_path;

	i = 0;
	//TODO waitpid
	dup2(pipex.tube[0], 1);
	dup2(pipex.fd2, 0);

	close(pipex.tube[1]);
	pipex.split_cmd = ft_split(cmd, ' ');
	while (pipex.all_paths[i])
	{
		temp_path = ft_join(pipex.all_paths[i], pipex.split_cmd[0]);
		if (access(temp_path, F_OK | X_OK) == 0)
		{
			err = execve(temp_path, &pipex.split_cmd[1], pipex.environment);
			if (err == -1)
			{
				free(temp_path);
				perror("parent execve");
			}
		}
	}
	close(pipex.tube[0]);
}

void	ft_pipex(t_pipex pipex, char **argv)
{
	/*
	 * fork (protected, obviously)
	 * set pid1 and pid2
	 * send child and parent functions with t_pipex and their respective commands
	 *      argv[2]   argv[3]
	 */

	int	spoon;

	spoon = fork();
	if (spoon < 0)
		perror("Fork:");
	if (spoon == 0)
	{
		//CHILD
		pipex.pid1 = spoon; //I THINK
		printf("I am the child! PID = %d\n", pipex.pid1);
		child(pipex, argv[2]);
	}
	else
	{
		//PARENT
		pipex.pid2 = spoon; //I THINK
		printf("I am the parent! PID = %d\n", pipex.pid2);
		parent(pipex, argv[3]);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	/* 
	 * ./pipex -- infile -- cmd1 -- cmd2 -- outfile
	 *    0          1       2        3        4
	 *
	 *
	 * make pipe
	 *
	 *	|__________________________|
	 *	|                          |
	 *	| [0] READ       WRITE [1] |
	 *	|__________________________|
	 *	|                          |
	 *
	 * check argc
	 * get path line and split into pipex.all_paths (ADD / AT THE END)
	 * open infile and outfile
	 * set pipex.environment to the envp
	 * pipex(pipex, argv)
	 */
	
	if (argc != 5)
	{
		printf("Please use only 4 arguments\n");
		//printf("infile	command1	command2	outfile\n");
		return (0);
	}
	pipe(pipex.tube);
	pipex.path = (string_finder(envp, "PATH=") + 5);
	pipex.all_paths = ft_joinall(ft_split(pipex.path, ':'), "/");
	pipex.fd1 = open(argv[1], O_RDONLY);
	pipex.fd2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	pipex.environment = envp;	
	ft_pipex(pipex, argv);
}
