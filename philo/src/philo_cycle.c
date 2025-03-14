/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:41:30 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 17:42:37 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	swap(int *left, int *right)
{
	int	tmp;

	tmp = *left;
	*left = *right;
	*right = tmp;
}

bool	locks_two_forks_in_order(t_philo *philo)
{
	int	right;
	int	left;

	left = philo->nb - 1;
	right = philo->nb % philo->sim->rules.nb_of_philo;
	if (left > right)
		swap(&left, &right);
	if (pthread_mutex_lock(&philo->sim->fork[left]) != SUCCESS)
	{

		log_status(philo, "cannot take a first fork");
		return (false);
	}
	log_status(philo, "has taken fork");
	if (pthread_mutex_lock(&philo->sim->fork[right]) != SUCCESS)
	{
		log_status(philo, "cannot take a second fork");
		return (false);
		pthread_mutex_unlock(&philo->sim->fork[left]);
		return (false);
	}
	log_status(philo, "has taken a second fork");
	return (true);
}

void	release_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->nb - 1;
	right = philo->nb % philo->sim->rules.nb_of_philo;
	if (left > right)
		swap(&left, &right);
	pthread_mutex_unlock(&philo->sim->fork[left]);
	pthread_mutex_unlock(&philo->sim->fork[right]);
}

void	start_eating(t_philo *philo)
{
	philo->state = IS_EATING;
	log_status(philo, "is eating");
	ft_usleep(philo, philo->sim->rules.time_to_eat);
	gettimeofday(&philo->last_meal, NULL);
}

void	start_sleeping(t_philo *philo)
{
	philo->state = IS_SLEEPING;
	log_status(philo, "is sleeping");
	ft_usleep(philo, philo->sim->rules.time_to_sleep);
}
