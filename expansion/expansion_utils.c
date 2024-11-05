/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:56:04 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/05 13:56:10 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isquote(char c)
{
	return (c == '\"' || c == '\'');
}

int	ft_isdelim(char c)
{
	return (c == ' ' || c == '\t');
}
