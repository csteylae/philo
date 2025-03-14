/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 19:32:49 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*start_dinner(void *arg)
{
	t_philo			*philo;

	philo = arg;
	init_last_meal(philo);
	if (philo->nb % 2 == 0)
		ft_usleep(philo, 1000);
	while (1)
	{
		if (!is_running(philo))
			return (NULL);
		if (locks_two_forks_in_order(philo))
		{
			if (!is_running(philo))
			{
				release_forks(philo);
				return (NULL);
			}
			start_eating(philo);
			if (!is_running(philo))
				return (NULL);
			philo->nb_of_meal++;
			release_forks(philo);
			if (!is_running(philo))
				return (NULL);
			start_sleeping(philo);
		}
		if (!is_running(philo))
			return (NULL);
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
		ret = pthread_create(&sim->philo[i].tid, NULL, &start_dinner, &sim->philo[i]);
		if (ret != SUCCESS)
			return ;
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
		printf("Error. Please enter:\n");
		printf("nb_of_philo, time_to_die, time_to_eat, time to_sleep, nb_of_meal(optionnal)\n");
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
