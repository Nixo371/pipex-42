#include "pipex.h"

size_t	ft_strlen(const char *str)
{
	const char	*s;

	s = str;
	while (*s)
		s++;
	return (s - str);
}

static char	**ft_free(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	while (i--)
		free(split[i]);
	free(split);
	return (NULL);
}

static int	seg_count(char const *s, char c)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (*s)
	{
		if (*s != c && n == 0)
		{
			n = 1;
			i++;
		}
		else if (*s == c && n == 1)
			n = 0;
		s++;
	}
	return (i);
}

static char	*seg_maker(char const *s, int start, int finish)
{
	char	*seg;
	int		i;

	i = 0;
	seg = malloc((finish - start + 1) * sizeof(char));
	if (seg == NULL)
		return (NULL);
	while (start < finish)
		seg[i++] = s[start++];
	seg[i] = '\0';
	return (seg);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		index;
	char	**split;

	split = malloc((seg_count(s, c) + 1) * sizeof(char *));
	if (!s || !split)
		return (0);
	i = 0;
	j = 0;
	index = -1;
	while (i++ <= ft_strlen(s))
	{
		if (s[i - 1] != c && index == -1)
			index = i - 1;
		else if ((s[i - 1] == c || (i - 1) == ft_strlen(s)) && index > -1)
		{
			split[j++] = seg_maker(s, index, (i - 1));
			if (split[j - 1] == NULL)
				return (ft_free(split));
			index = -1;
		}
	}
	split[j] = 0;
	return (split);
}
