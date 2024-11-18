/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cidr_converter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otodd <otodd@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:01:21 by otodd             #+#    #+#             */
/*   Updated: 2024/11/13 17:12:45 by otodd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
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

static char	*ipv4_do_mask(char *str, char *str2)
{
	size_t	c;
	int		ip[4];
	char	*head;
	char	**ipv4;
	char	**a_head;
	int		n;
	char	*head2;
	char	**mask;
	char	**a_head2;
	int		n2;
	int		i;
	char	*s;

	c = 0;
	head = str;
	head2 = str2;
	while (*head)
	{
		if (*head == '.' && (*(head + 1) && *(head + 1) != '.')
			&& (*head >= '0' || *head <= '9' || *head == '0'))
			c++;
		head++;
	}
	if (c != 3)
		return (NULL);
	c = 0;
	head = str;
	while (*head2)
	{
		if (*head2 == '.' && (*(head2 + 1) && *(head2 + 1) != '.')
			&& (*head2 >= '0' || *head2 <= '9' || *head2 == '0'))
			c++;
		head2++;
	}
	if (c != 3)
		return (NULL);
	ipv4 = ft_split(str, '.');
	mask = ft_split(str2, '.');
	a_head = ipv4;
	a_head2 = mask;
	i = 0;
	while (*ipv4 && *mask)
	{
		n = atoi(*ipv4);
		n2 = atoi(*mask);
		if ((n > 255 && n != -1) && (n2 > 255 && n2 != -1))
		{
			ft_free_array(a_head, 4);
			free(a_head);
			ft_free_array(a_head2, 4);
			free(a_head2);
			return (NULL);
		}
		ip[i] = n & n2;
		ipv4++;
		mask++;
		i++;
	}
	s = malloc(sizeof(char *) * 1);
	sprintf(s, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	return (s);
}

static char	*cidr_do(int n)
{
	char	*s;
	int		i;
	int		mask[4] = {0, 0, 0 ,0};
	int		byte;
	int		bit;

	i = 0;
	while (i < n)
	{
		byte = i / 8;
		bit = 7 - (i % 8);
		mask[byte] |= (1 << bit);
		i++;
	}

	s = malloc(sizeof(char *) * 1);
	sprintf(s, "%d.%d.%d.%d", mask[0], mask[1], mask[2], mask[3]);
	return (s);
}

int	main(int argc, char **argv)
{
	char	*ipv4_str;
	char	*mask_str;
	int		ret;
	int		i;
	char	*ret2;

	if (argc == 1)
		return (EXIT_FAILURE);
	if (argc == 3)
	{
		ipv4_str = argv[1];
		mask_str = argv[2];
		ret2 = ipv4_do_mask(ipv4_str, mask_str);
		if (!ret2)
		{
			printf("%s is not a valid ipv4\n", ipv4_str);
			return (EXIT_FAILURE);
		}
		printf("Masked IP: %s\n", ret2);
		free(ret2);
		mask_str = argv[2];
		ret = ipv4_do(mask_str);
		if (ret == -1)
		{
			printf("%s is not a valid mask\n", mask_str);
			return (EXIT_FAILURE);
		}
		printf("%s has a cidr notation: /%d\n", mask_str, ret);
		printf("Number of possible hosts: %d\n", ((int)pow(2, 32 - ret) - 2));
	}
	else if (argc == 2)
	{
		ipv4_str = argv[1];
		if (ipv4_str[0] == '/')
		{
			mask_str = cidr_do(atoi(&ipv4_str[1]));
			printf("%s has a subnet mask: %s\n", ipv4_str, mask_str);
			free(mask_str);
		}
		else if (strcmp(ipv4_str, "s_range") == 0)
		{
			i = 0;
			while (i <= 32)
			{
				mask_str = cidr_do(i);
				printf("/%d has a subnet mask: %s\n", i, mask_str);
				free(mask_str);
				i++;
			}
		}
		else if (strcmp(ipv4_str, "s_range_h") == 0)
		{
			i = 0;
			while (i <= 32)
			{
				mask_str = cidr_do(i);
				printf("/%d has a subnet mask: %s\n", i, mask_str);
				ret = ipv4_do(mask_str);
				if (ret == -1)
				{
					printf("%s is not a valid ipv4\n", ipv4_str);
					return (EXIT_FAILURE);
				}
				printf("Number of possible hosts: %d\n\n", ((int)pow(2, 32 - ret) - 2));
				free(mask_str);
				i++;
			}
		}
		else
		{
			ret = ipv4_do(ipv4_str);
			if (ret == -1)
			{
				printf("%s is not a valid ipv4\n", ipv4_str);
				return (EXIT_FAILURE);
			}
			printf("%s has a cidr notation: /%d\n", ipv4_str, ret);
			printf("Number of possible hosts: %d\n", ((int)pow(2, 32 - ret) - 2));
		}
	}
	return (EXIT_SUCCESS);
}
