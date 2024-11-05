/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:29:35 by jhirvone          #+#    #+#             */
/*   Updated: 2024/04/24 11:29:36 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*free_array(char **array, int n)
{
	int	i;

	i = 0;
	while (i <= n)
	{
		free (array[i]);
		i++;
	}
	free(array);
	return (0);
}

static int	count_words(const char *str, char sepa)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (str[i])
	{
		if (str[i] != sepa && (str[i + 1] == sepa || str[i + 1] == 0))
			words++;
		i++;
	}
	return (words);
}

static char	**ft_merge(char **strs, const char *str, char sepa)
{
	int	i;
	int	j;
	int	n;

	i = 0;
	n = 0;
	while (str[i])
	{
		if (str[i] == sepa)
		{
			i++;
			continue ;
		}
		j = 0;
		while (str[i + j] != sepa && str[i + j])
			j++;
		strs[n] = ft_substr(str, i, j);
		if (!(strs[n]))
			return (free_array(strs, n));
		n++;
		i += j;
	}
	strs[n] = 0;
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**strs;

	words = count_words(s, c);
	strs = (char **)malloc(sizeof(char *) * (words + 1));
	if (!strs)
		return (0);
	strs = ft_merge(strs, s, c);
	return (strs);
}
