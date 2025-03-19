/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/19 18:16:30 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->fork[0]);
	log_status(philo, "has taken a fork");
	while (is_running(philo))
		ft_usleep(philo, 10);
	pthread_mutex_unlock(&philo->sim->fork[0]);
	return (NULL);
}

void	*start_dinner(void *arg)
{
	t_philo			*philo;

	philo = arg;
	init_last_meal(philo);
	if (philo->nb % 2 == 0)
		ft_usleep(philo, 1);
	while (is_running(philo))
	{
		if (philo->sim->rules.nb_of_philo == 1)
			return (handle_single_philo(philo));
		if (is_running(philo) && nb_of_meal_reached(philo))
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
	}
	return (NULL);
}

void	end_of_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i != sim->rules.nb_of_philo)
	{
		pthread_join(sim->philo[i].tid, NULL);
		i++;
	}
}

void	launch_simulation(t_simulation *sim)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
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
	monitoring(sim);
	end_of_simulation(sim);
}

int	main(int ac, char **argv)
{
	t_simulation	sim;

	if (ac != 5 && ac != 6)
	{
		printf(PARAM_NB_ERROR);
		return (EXIT_FAILURE);
	}
	if (!setup_dinner_table(argv, &sim))
	{
		terminate_simulation(&sim);
		return (EXIT_FAILURE);
	}
	launch_simulation(&sim);
	terminate_simulation(&sim);
	return (EXIT_SUCCESS);
}
