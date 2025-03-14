/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_running_conditions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:46:44 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 19:44:45 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool is_running(t_philo *philo)
{
	bool	is_running;

	pthread_mutex_lock(&philo->sim->run_check);
	is_running = philo->sim->is_running;
	pthread_mutex_unlock(&philo->sim->run_check);
	return (is_running);
}

void	monitoring(t_simulation *sim)
{
	int		i;
	long	current_time;
	bool	is_dead;
	bool	meal_nb_reached;
	
	i = 0;
	is_dead = false;
	meal_nb_reached = false;
	while (1)
	{
		current_time = get_timestamp_ms(sim);
		if (i >= sim->rules.nb_of_philo)
			i = 0;
		if ((current_time - sim->philo[i].last_meal) >= sim->rules.time_to_die)
		{
			is_dead = true;
			break ;
		}
		if (sim->rules.nb_of_meal != UNLIMITED_MEAL &&
			sim->philo[i].nb_of_meal == sim->rules.nb_of_meal)
		{
			meal_nb_reached = true;
		}
		i++;
	}
	pthread_mutex_lock(&sim->run_check);
	sim->is_running = false;
	pthread_mutex_unlock(&sim->run_check);
}


