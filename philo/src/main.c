/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/25 17:30:39 by csteylae         ###   ########.fr       */
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
	while (!is_running(philo))
		ft_usleep(philo, 100);
	init_last_meal(philo);
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
		{
			int time = ((philo->sim->rules.time_to_eat / 2));
			ft_usleep(philo, time);
		}
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
	if (!get_rules(argv, &sim.rules))
	{
		printf("Error. Invalid parameter\n");
		return (EXIT_FAILURE);
	}
	if (ac == 6 && sim.rules.nb_of_meal == 0)
	{
		printf("Simulation ends immediately: philos needs to eat 0 times\n");
		exit(EXIT_SUCCESS);
	}
	if (!setup_dinner_table(&sim))
	{
		terminate_simulation(&sim);
		return (EXIT_FAILURE);
	}
	launch_simulation(&sim);
	terminate_simulation(&sim);
	return (EXIT_SUCCESS);
}
