/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:46:44 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/25 15:20:59 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool	is_running(t_philo *philo)
{
	bool	is_running;

	pthread_mutex_lock(&philo->sim->run_check);
	is_running = philo->sim->is_running;
	pthread_mutex_unlock(&philo->sim->run_check);
	return (is_running);
}

bool	dies_from_starvation(t_philo *philo)
{
	long	time_to_die;
	long	current_time;
	bool	dead;
	long	last_meal;

	time_to_die = philo->sim->rules.time_to_die;
	current_time = get_timestamp_ms(philo->sim);
	dead = false;
	pthread_mutex_lock(&philo->state);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->state);
	if (current_time - last_meal >= time_to_die)
	{
		pthread_mutex_lock(&philo->sim->death_check);
		philo->sim->is_dead = true;
		pthread_mutex_unlock(&philo->sim->death_check);
		dead = true;
	}
	return (dead);
}

bool	nb_of_meal_reached(t_philo *philo)
{
	int	nb_of_meal;

	nb_of_meal = 0;
	if (philo->sim->rules.nb_of_meal == UNLIMITED_MEAL)
		return (false);
	pthread_mutex_lock(&philo->state);
	nb_of_meal = philo->nb_of_meal;
	pthread_mutex_unlock(&philo->state);
	if (nb_of_meal == philo->sim->rules.nb_of_meal)
		return (true);
	return (false);
}

bool	all_meals_goal_check(t_simulation *sim)
{
	int	i;

	i = 0;
	if (sim->rules.nb_of_meal == UNLIMITED_MEAL)
		return (false);
	while (i != sim->rules.nb_of_philo)
	{
		if (!nb_of_meal_reached(&sim->philo[i]))
			return (false);
		i++;
	}
	return (true);
}

void	monitoring(t_simulation *sim)
{
	int	i;
	long time_of_death;

	i = 0;
	while (1)
	{
		i = 0;
		while (i != sim->rules.nb_of_philo)
		{
			if (dies_from_starvation(&sim->philo[i]))
			{
				time_of_death = get_timestamp_ms(sim);
				pthread_mutex_lock(&sim->write_msg);
				printf("%lu philo %i died\n", time_of_death, sim->philo[i].nb);
				pthread_mutex_unlock(&sim->write_msg);
				pthread_mutex_lock(&sim->run_check);
				sim->is_running = false;
				pthread_mutex_unlock(&sim->run_check);
				return ;
			}
			if (all_meals_goal_check(sim))
			{
				pthread_mutex_lock(&sim->run_check);
				sim->is_running = false;
				pthread_mutex_unlock(&sim->run_check);
				return ;
			}
			i++;
		}
		usleep(1000);
	}
}
