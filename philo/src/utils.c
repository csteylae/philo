/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:19:48 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/27 16:02:59 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	ft_usleep(t_philo *philo, long time_in_ms)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_timestamp_ms(philo->sim);
	elapsed = 0;
	remaining = 0;
	while (elapsed < time_in_ms)
	{
		if (elapsed % 50 == 0 && !is_running(philo))
			return ;
		remaining = time_in_ms - elapsed;
		if (remaining < 100)
			usleep(50);
		else
			usleep(500);
		elapsed = get_timestamp_ms(philo->sim) - start;
	}
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
	bool			is_dead;

	pthread_mutex_lock(&philo->sim->death_check);
	is_dead = philo->sim->is_dead;
	pthread_mutex_unlock(&philo->sim->death_check);
	if (is_dead)
		return ;
	timestamp_in_ms = get_timestamp_ms(philo->sim);
	pthread_mutex_lock(&philo->sim->write_msg);
	printf("%lu philo %i %s\n", timestamp_in_ms, philo->nb, str);
	pthread_mutex_unlock(&philo->sim->write_msg);
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
