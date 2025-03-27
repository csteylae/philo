/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:41:30 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/27 12:32:33 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

bool	locks_two_forks_in_order(t_philo *philo)
{
	int	right;
	int	left;
	int	first;
	int	second;

	left = philo->nb - 1;
	right = philo->nb % philo->sim->rules.nb_of_philo;
	if (philo->nb % 2 == 0)
	{
		first = right;
		second = left;
	}
	else
	{
		first = left;
		second = right;
	}
	pthread_mutex_lock(&philo->sim->fork[first]);
	log_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->sim->fork[second]);
	log_status(philo, "has taken a second fork");
	return (true);
}

void	release_forks(t_philo *philo)
{
	int	left;
	int	right;
	int	first;
	int	second;

	left = philo->nb - 1;
	right = philo->nb % philo->sim->rules.nb_of_philo;
	if (philo->nb % 2 == 0)
	{
		first = right;
		second = left;
	}
	else
	{
		first = left;
		second = right;
	}
	pthread_mutex_unlock(&philo->sim->fork[first]);
	pthread_mutex_unlock(&philo->sim->fork[second]);
}

void	start_eating(t_philo *philo)
{
	log_status(philo, "is eating");
	pthread_mutex_lock(&philo->state);
	philo->last_meal = get_timestamp_ms(philo->sim);
	philo->nb_of_meal++;
	pthread_mutex_unlock(&philo->state);
	ft_usleep(philo, philo->sim->rules.time_to_eat);
}

void	start_sleeping(t_philo *philo)
{
	log_status(philo, "is sleeping");
	ft_usleep(philo, philo->sim->rules.time_to_sleep);
}
