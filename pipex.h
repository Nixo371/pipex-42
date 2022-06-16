#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdarg.h>
# include <stddef.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int	tube[2];
	int	fd1;
	int	fd2;
	int	pid1;
	int	pid2;
	char	*path;
	char	**all_paths;
	char	**split_cmd;
	char	**environment;
}	t_pipex;

void	ft_pipex(t_pipex pipex, char **argv);
void	parent(t_pipex pipex, char *cmd);
void	child(t_pipex pipex, char *cmd);

char	*string_finder(char **ptr, char *str);
char	*ft_join(char *s1, char *s2);
char	**ft_joinall(char **ptr, char *s2);
char	**ft_freeall(char **ptr, int i);

char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *str);

#endif
