/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_launch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:22:00 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/27 15:53:11 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	*handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->fork[0]);
	log_status(philo, "has taken a fork");
	while (is_running(philo))
		ft_usleep(philo, 10);
	pthread_mutex_unlock(&philo->sim->fork[0]);
	return (NULL);
}

static void	wait_everyone(t_philo *philo)
{
	while (!is_running(philo))
		ft_usleep(philo, 1);
}

static void	setup_initial_condition(t_philo *philo)
{
	wait_everyone(philo);
	init_last_meal(philo);
	if (philo->nb % 2 != 0)
		ft_usleep(philo, philo->sim->rules.time_to_eat / 2);
}

static void	*start_dinner(void *arg)
{
	t_philo			*philo;

	philo = arg;
	setup_initial_condition(philo);
	if (philo->sim->rules.nb_of_philo == 1)
		return (handle_single_philo(philo));
	while (is_running(philo))
	{
		if (nb_of_meal_reached(philo))
		{
			ft_usleep(philo, 100);
			continue ;
		}
		if (locks_two_forks_in_order(philo))
		{
			start_eating(philo);
			release_forks(philo);
			start_sleeping(philo);
		}
		log_status(philo, "is thinking");
		if (philo->sim->rules.nb_of_philo % 2 != 0)
			ft_usleep(philo, 10);
	}
	return (NULL);
}

void	launch_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	if (gettimeofday(&sim->starting_time, NULL) != SUCCESS)
	{
		printf("error\n");
		return ;
	}
	while (i != sim->rules.nb_of_philo)
	{
		pthread_create(&sim->philo[i].tid, NULL, &start_dinner, &sim->philo[i]);
		i++;
	}
	usleep(1000);
	pthread_mutex_lock(&sim->run_check);
	sim->is_running = true;
	gettimeofday(&sim->starting_time, NULL);
	pthread_mutex_unlock(&sim->run_check);
	monitoring(sim);
	end_simulation(sim);
}
