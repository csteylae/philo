/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_sim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:40:20 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/20 15:43:34 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	destroy_all_mutex(pthread_mutex_t *array, int len)
{
	int	i;

	i = 0;
	while (i != len)
	{
		pthread_mutex_destroy(&array[i]);
		i++;
	}
}

void	terminate_simulation(t_simulation *sim)
{
	free(sim->philo);
	destroy_all_mutex(sim->fork, sim->rules.nb_of_philo);
	free(sim->fork);
	pthread_mutex_destroy(&sim->write_msg);
	pthread_mutex_destroy(&sim->run_check);
	pthread_mutex_destroy(&sim->death_check);
	pthread_mutex_destroy(&sim->meal_nb_check);
}
