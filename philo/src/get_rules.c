/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_rules.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:06:38 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 14:43:49 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static bool	is_digit(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
	{
		return (false);
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

static int	ft_atol(char *str)
{
	size_t	i;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i])
	{
		ret *= 10;
		ret += (str[i] - '0');
		i++;
		if (ret < 0)
		{
			return (FAIL);
		}
	}
	return (ret);
}

static int	init_var(char *str)
{
	int	ret;

	ret = 0;
	if (!is_digit(str))
		return (-1);
	ret = ft_atol(str);
	return (ret);
}

bool	get_rules(char **argv, t_rules *rule)
{
	rule->nb_of_philo = init_var(argv[1]);
	if (rule->nb_of_philo <= 0)
		return (false);
	rule->time_to_die = init_var(argv[2]);
	if (rule->time_to_die <= 0)
		return (false);
	rule->time_to_eat = init_var(argv[3]);
	if(rule->time_to_eat <= 0)
		return (false);
	rule->time_to_sleep = init_var(argv[4]);
	if (rule->time_to_sleep <= 0)
		return (false);
	if (argv[5])
		rule->nb_of_meal = init_var(argv[5]);
	else
		rule->nb_of_meal = UNLIMITED_MEAL;
	if (rule->nb_of_meal < 0)
		return (false);
	return (true);
}
