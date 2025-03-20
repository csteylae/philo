/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:22:05 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/20 15:41:22 by csteylae         ###   ########.fr       */
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
		printf("Error : malloc\n");
		return (false);
	}
	while (i != sim->rules.nb_of_philo)
	{
		sim->philo[i].nb = i + 1;
		sim->philo[i].state = HUNGRY;
		sim->philo[i].sim = sim;
		sim->philo[i].nb_of_meal = 0;
		sim->philo[i].last_meal = 0;
		i++;
	}
	return (true);
}

static bool	init_mutex(pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
	{
		write(STDERR_FILENO, MUTEX_INIT_ERROR, ft_strlen(MUTEX_INIT_ERROR));
		return (false);
	}
	return (true);
}

static bool	init_fork(t_simulation *sim)
{
	int	i;

	i = 0;
	sim->fork = malloc(sizeof(pthread_mutex_t) * sim->rules.nb_of_philo);
	if (!sim->fork)
	{
		free(sim->philo);
		write(STDERR_FILENO, MALLOC_ERROR, ft_strlen(MALLOC_ERROR));
		return (false);
	}
	while (i != sim->rules.nb_of_philo)
	{
		if (!init_mutex(&sim->fork[i]))
		{
			destroy_all_mutex(sim->fork, i);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	setup_dinner_table(t_simulation *sim)
{
	if (!init_philo(sim))
		return (false);
	if (!init_fork(sim))
		return (false);
	if (!init_mutex(&sim->write_msg))
		return (false);
	if (!init_mutex(&sim->death_check))
		return (false);
	if (!init_mutex(&sim->run_check))
		return (false);
	if (!init_mutex(&sim->meal_nb_check))
		return (false);
	sim->is_dead = false;
	sim->is_running = true;
	return (true);
}
