/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:26:09 by jhirvone          #+#    #+#             */
/*   Updated: 2024/04/24 11:43:21 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numbers(int nbr)
{
	int	result;

	if (!nbr)
		return (1);
	result = 0;
	if (nbr < 0)
		result++;
	while (nbr)
	{
		nbr /= 10;
		result++;
	}
	return (result);
}

static void	ft_make_string(char *dest, int nbr, int index)
{
	if (nbr >= 10)
	{
		ft_make_string(dest, nbr / 10, index - 1);
		ft_make_string(dest, nbr % 10, index);
	}
	else
		dest[index] = nbr + '0';
}

char	*ft_itoa(int n)
{
	int		size;
	char	*dest;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	size = ft_numbers(n);
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (!dest)
		return (0);
	if (n < 0)
	{
		dest[0] = '-';
		n = -n;
	}
	dest[size] = '\0';
	ft_make_string(dest, n, size - 1);
	return (dest);
}
