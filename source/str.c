#include "../include/str.h"

size_t	ft_strlen(const char *str)
{
	const char	*s;

	s = str;
	while (*s)
		s++;
	return (s - str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*ft_strcpy(char *dst, const char *src)
{
	char *d = dst;

	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return d;
}

char	*ft_strdup(const char *s)
{
	char	*str;

	size_t len = ft_strlen(s) + 1;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_strcpy(str, s);
	return (str);
}
