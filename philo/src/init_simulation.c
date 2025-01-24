/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:22:05 by csteylae          #+#    #+#             */
/*   Updated: 2025/01/23 18:16:07 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static bool	init_philo(t_simulation *sim)
{
	int	i;

	i = 0;
	sim->philo = malloc(sizeof(*sim->philo) * sim->rules.nb_of_philo);
	if (!sim->philo)
	{
		perror("malloc");
		return (false);
	}
	while (i != sim->rules.nb_of_philo)
	{
		sim->philo[i].nb = i;
		sim->philo[i].state = HUNGRY;
		sim->philo[i].sim = sim;
		i++;
	}
	return (true);
}

bool	init_mutex(t_simulation *sim)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	while (i != sim->rules.nb_of_philo)
	{
		check = pthread_mutex_init(&sim->fork[i], NULL);
		if (check != 0)
		{
			//free evrything();
			write(STDERR_FILENO, "mutex error", ft_strlen("mutex error"));
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	init_fork(t_simulation *sim)
{
	sim->fork = malloc(sizeof(pthread_mutex_t) * sim->rules.nb_of_philo);
	if (!sim->fork)
	{
	//	free(sim->philo);
		perror("malloc");
		return (false);
	}
	if (!init_mutex(sim))
		return (false);
	return (true);
}

bool	setup_dinner_table(char **argv, t_simulation *sim)
{
	t_rules	rules;
	int		check;

	check = 0;
	if (!get_rules(argv, &rules))
		return (false);
	sim->rules = rules;
	if (!init_philo(sim))
		return (false);
	if (!init_fork(sim)) 
		return (false);
	check = pthread_mutex_init(&sim->death_check, NULL);
	if (check != 0)
	{
		//free ervytting
		return (false);
	}
	sim->is_dead = false;
	return (true);
}
