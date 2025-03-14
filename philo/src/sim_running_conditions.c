/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_running_conditions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:46:44 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 18:25:46 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static bool	nb_of_meal_reached(t_philo *philo, int nb_of_meal)
{
	if (philo->sim->rules.nb_of_meal != UNLIMITED_MEAL)
	{
		if (nb_of_meal == philo->sim->rules.nb_of_meal)
			return (true);
		return (false);
	}
	return (false);
}

void	is_running(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->run_check);
	if (nb_of_meal_reached(philo, philo->nb_of_meal))
		philo->sim->is_running = false;
	if (philo->sim->is_dead)
		philo->sim->is_running = false;
	pthread_mutex_unlock(&philo->sim->run_check);
}

long	convert_in_ms(t_philo *philo)
{
	long	sec;
	long	usec;

	sec = philo->last_meal.tv_sec - philo->sim->starting_time.tv_sec;
	usec = philo->last_meal.tv_usec - philo->sim->starting_time.tv_usec;
	if (usec < 0)
	{
		sec -= 1;
		usec += 1000000;
	}
	return ((sec * 1000) * (usec / 1000));
}

bool	time_to_die_passed_since_last_meal(t_philo *philo)
{
	long	timestamp;
	long	last_meal_ms;

	timestamp = get_timestamp_ms(philo->sim);
	last_meal_ms = convert_in_ms(philo);
	if ((timestamp - last_meal_ms) >= philo->sim->rules.time_to_die)
		return (true);
	return (false);
}

void	monitoring(t_simulation *sim)
{
	int	i;
	
	i = 0;
	while (1)
	{
		if (i >= sim->rules.nb_of_philo)
			i = 0;
		if (time_to_die_passed_since_last_meal(&sim->philo[i]))
		{
			pthread_mutex_lock(&sim->death_check);
			sim->is_dead = true;
			pthread_mutex_unlock(&sim->death_check);
			log_status(&sim->philo[i], "died");
			return ;
		}
		i++;
	}
}
