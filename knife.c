#include "pipex.h"



char	*ft_join(char *s1, char *s2, char c)
{
	//join both strings with the char in between
	//	str1 c str2
	char	*str;
	int	i;
	int	j;

	i = 0;
	j = 0;
	str = malloc((ft_strlen(s1) + ft_strlen(s2) + 2) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = c;
	i++;
	j = i;
	i = 0;
	while (s2[i])
	{
		str[j] = s2[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

void	child(int fd, char * const *command, char **env)
{
	int	i;
	int	tube[2];
	char	*cmd_path;
	char	**the_paths;

	i = 0;
	pipe(tube);
	dup2(fd, 0);
	dup2(tube[1], 1);
	close(tube[0]);
	close(fd);
	the_paths = ft_split(find_str(env, "PATH") + 5, ':');
	while(the_paths[i])
	{
		cmd_path = ft_join(the_paths[i], command[0], '/'); 
		if (cmd_path == NULL)
			exit(EXIT_FAILURE);
		execve(cmd_path, command, env);
	}
}

void	parent(int fd, char * const *command, char **env)
{
	int	i;
	int	tube[2];
	char	*cmd_path;
	char	**the_paths;

	i = 0;
	pipe(tube);
	waitpid(-1, &status, 0);
	dup2(tube[0], 0);
	dup2(fd, 1);
	close(tube[1]);
	close(fd);
	the_paths = ft_split(find_str(env, "PATH") + 5, ':');
	while(the_paths[i])
	{
		cmd_path = ft_join(the_paths[i], command[0], '/');
		if(cmd_path == NULL)
			exit(EXIT_FAILURE);
		execve(cmd_path, command, env);
	}
}
