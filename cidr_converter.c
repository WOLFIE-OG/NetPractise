/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cidr_converter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otodd <otodd@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:01:21 by otodd             #+#    #+#             */
/*   Updated: 2024/11/11 19:33:55 by otodd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <bsd/string.h>
#include <stdlib.h>
#include <stdbool.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_string;
	size_t	size;

	if (!s)
		return (NULL);
	if (start >= strlen(s))
		return (strdup(""));
	size = strlen(s + start);
	if (size < len)
		len = size;
	new_string = malloc(sizeof(char) * (len + 1));
	if (!new_string)
		return (NULL);
	strlcpy(new_string, s + start, len + 1);
	return (new_string);
}

static size_t	total_string_length(const char *s, char c)
{
	size_t	result;

	result = 0;
	while (*s)
	{
		if (*s != c)
		{
			result++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**string_list;
	size_t	index;
	size_t	len;

	if (!s)
		return (NULL);
	index = 0;
	string_list = malloc(sizeof(char *) * (total_string_length(s, c) + 1));
	if (!string_list)
		return (NULL);
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (*s && *s != c && ++len)
				s++;
			string_list[index++] = ft_substr(s - len, 0, len);
		}
		else
			s++;
	}
	string_list[index] = NULL;
	return (string_list);
}

void	ft_free_array(char **array, size_t len)
{
	while (len)
		free(array[--len]);
}

static int	ipv4_do(char *str)
{
	size_t	c;
	char	*head;
	char	**ipv4;
	char	**a_head;
	int		n;
	int		i;
	int		count;

	c = 0;
	head = str;
	count = 0;
	while (*head)
	{
		if (*head == '.' && (*(head + 1) && *(head + 1) != '.')
			&& (*head >= '0' || *head <= '9' || *head == '0'))
			c++;
		head++;
	}
	if (c != 3)
		return (-1);
	ipv4 = ft_split(str, '.');
	a_head = ipv4;
	while (*ipv4)
	{
		n = atoi(*ipv4);
		if (n > 255 && n != -1)
		{
			ft_free_array(a_head, 4);
			free(a_head);
			return (-1);
		}
		i = 8;
		while (i > 0)
		{
			i--;
			if (((n >> i) % 2) != 0)
				count++;
		}
		ipv4++;
	}
	return (count);
}

int	main(int argc, char **argv)
{
	char	*ipv4_str;
	int		ret;

	if (argc == 1)
		return (EXIT_FAILURE);
	ipv4_str = argv[1];
	ret = ipv4_do(ipv4_str);
	if (ret == -1)
	{
		printf("%s is not a valid ipv4\n", ipv4_str);
		return (EXIT_FAILURE);
	}
	printf("%s has a subnet mask: /%d\n", ipv4_str, ret);
	return (EXIT_SUCCESS);
}
