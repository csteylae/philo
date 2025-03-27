/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_sim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:40:20 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/27 16:02:37 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	stop_running(t_simulation *sim)
{
	pthread_mutex_lock(&sim->run_check);
	sim->is_running = false;
	pthread_mutex_unlock(&sim->run_check);
}

void	end_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i != sim->rules.nb_of_philo)
	{
		pthread_join(sim->philo[i].tid, NULL);
		i++;
	}
}

void	destroy_forks(pthread_mutex_t *array, int len)
{
	int	i;

	i = 0;
	while (i != len)
	{
		pthread_mutex_destroy(&array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	free_philo_array(t_philo *array, int len)
{
	int	i;

	i = 0;
	while (i != len)
	{
		pthread_mutex_destroy(&array[i].state);
		i++;
	}
	free(array);
	array = NULL;
}

void	terminate_simulation(t_simulation *sim)
{
	free_philo_array(sim->philo, sim->rules.nb_of_philo);
	destroy_forks(sim->fork, sim->rules.nb_of_philo);
	pthread_mutex_destroy(&sim->write_msg);
	pthread_mutex_destroy(&sim->run_check);
	pthread_mutex_destroy(&sim->death_check);
}
