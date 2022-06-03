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

int	last_arg(char	**args);
void	pipex(int fd1, int fd2, char **arguments, char **env);
void	child(int fd, char * const *command, char **env);
void	parent(int fd, char * const * command, char **env);
char	*ft_join(char *s1, char *s2, char c);
char	*find_str(char **ptr, char *crit);

#endif
