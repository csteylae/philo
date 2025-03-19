/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_running_conditions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:46:44 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/19 18:20:29 by csteylae         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->sim->death_check);
	last_meal = philo->last_meal;
	if (current_time - last_meal >= time_to_die)
	{
		philo->sim->is_dead = true;
		dead = true;
	}
	pthread_mutex_unlock(&philo->sim->death_check);
	return (dead);
}

bool	nb_of_meal_reached(t_philo *philo)
{
	int	nb_of_meal;

	nb_of_meal = 0;
	if (philo->sim->rules.nb_of_meal == UNLIMITED_MEAL)
		return (false);
	pthread_mutex_lock(&philo->sim->meal_nb_check);
	nb_of_meal = philo->nb_of_meal;
	pthread_mutex_unlock(&philo->sim->meal_nb_check);
	if (nb_of_meal == philo->sim->rules.nb_of_meal)
		return (true);
	return (false);
}

bool	all_meals_goal_check(t_simulation *sim)
{
	int	i;
	int	nb_of_meal;

	i = 0;
	nb_of_meal = 0;
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
	int		i;
	long	time_of_death;

	i = 0;
	while (1)
	{
		if (i >= sim->rules.nb_of_philo)
			i = 0;
		if (dies_from_starvation(&sim->philo[i]))
		{
			time_of_death = get_timestamp_ms(sim);
			pthread_mutex_lock(&sim->write_msg);
			printf("%lu philo %i died\n", time_of_death, sim->philo[i].nb);
			pthread_mutex_unlock(&sim->write_msg);
			break ;
		}
		if (all_meals_goal_check(sim))
			break ;
		i++;
	}
	pthread_mutex_lock(&sim->run_check);
	sim->is_running = false;
	pthread_mutex_unlock(&sim->run_check);
}
