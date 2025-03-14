/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:19:48 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 19:46:49 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	ft_usleep(t_philo *philo, long time_in_ms)
{
	long	start;
	long	current_timestamp;

	start = get_timestamp_ms(philo->sim);
	while (1)
	{
		current_timestamp = get_timestamp_ms(philo->sim);
		if (current_timestamp >= (start + time_in_ms))
			break ;
		usleep(100);
	}
	return ;
}

long	get_timestamp_ms(t_simulation *sim)
{
	struct timeval	current_time;
	long			sec_diff;
	long			usec_diff;
	long			timestamp_ms;

	if (gettimeofday(&current_time, NULL) != SUCCESS)
	{
		return (-1);
	}
	sec_diff = current_time.tv_sec - sim->starting_time.tv_sec;
	usec_diff = current_time.tv_usec - sim->starting_time.tv_usec;
	if (usec_diff < 0)
	{
		sec_diff -= 1;
		usec_diff += 1000000;
	}
	timestamp_ms = (sec_diff * 1000) + (usec_diff / 1000);
	return (timestamp_ms);
}

void	log_status(t_philo *philo, char *str)
{
	long			timestamp_in_ms;

	timestamp_in_ms = get_timestamp_ms(philo->sim);
	pthread_mutex_lock(&philo->sim->write_msg);
	if (philo->sim->is_running)
		printf("%lu philo %i %s\n", timestamp_in_ms, philo->nb, str);
	pthread_mutex_unlock(&philo->sim->write_msg);
}

void	init_last_meal(t_philo *philo)
{
	philo->last_meal = get_timestamp_ms(philo->sim);
}

void	terminate_simulation(t_simulation *sim)
{
	free(sim->philo);
	free(sim->fork);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}
