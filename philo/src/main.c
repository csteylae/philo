/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 18:26:57 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*start_dinner(void *arg)
{
	t_philo			*philo;

	philo = arg;
	init_last_meal(philo);
	if (philo->nb % 2 == 0)
	{
		ft_usleep(philo, 1000);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->sim->run_check);
		if (!philo->sim->is_running)
		{
			pthread_mutex_unlock(&philo->sim->run_check);
			break ;
		}
		pthread_mutex_unlock(&philo->sim->run_check);
		if (locks_two_forks_in_order(philo))
		{
			start_eating(philo);
			philo->nb_of_meal++;
			release_forks(philo);
			start_sleeping(philo);
		}
		log_status(philo, "is thinking");
	}
	return (NULL);
}

void	launch_simulation(t_simulation *sim)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	if (gettimeofday(&sim->starting_time, NULL) != SUCCESS)
	{
		printf("error:gettimeofday before starting of dinner\n");
		return ;
	}
	while (i != sim->rules.nb_of_philo)
	{
		ret = pthread_create(&sim->philo[i].tid, NULL, &start_dinner, &sim->philo[i]);
		if (ret != SUCCESS)
		{
			//error_handling
			return ;
		}
		i++;
	}
	monitoring(sim);
	i = 0;
	ret = 0;
	while (i != sim->rules.nb_of_philo)
	{
		pthread_join(sim->philo[i].tid, NULL);
		i++;
	}
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
