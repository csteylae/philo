/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/01/24 16:24:39 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	print_rules(t_rules rule)
{
	printf("nb of philo : %i\n", rule.nb_of_philo);
	printf("time_to_die : %i\n", rule.time_to_die);
	printf("time_to_eat : %i\n", rule.time_to_eat);
	printf("time_to_sleep : %i\n", rule.time_to_sleep);
	printf("nb_of_meal : %i\n", rule.nb_of_meal);
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

void	*start_routine(void *arg)
{
	t_philo	*philo;
	int		nb_of_meal;

	philo = arg;
	nb_of_meal = 0;
	if (philo->nb % 2 == 0)
	{
		usleep(1000);
	}
	while (true)
	{
		if (nb_of_meal == sim->nb_of_meal && sim->nb_of_meal != UNLIMITED_MEAL)
			break;
		takes_two_fork(philo);
		start_eating();
		release_forks();
		start_sleeping();
		start_thinking();
		nb_of_meal++;
	}
	return (NULL);
	// the unit time are expressed in milliseconds
	//eat() => take rigt and left fork, one in each hand. every philo need to eat
	//when a philo has finished eating, they put their forks back on the table and start sleeping
	// once awake, theyr dtart thinking again.
	// the simulation stop when a philosopher dies of starvation
	//sleep();
}

void	launch_simulation(t_simulation *sim)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	while (i != sim->rules.nb_of_philo)
	{
		check = pthread_create(&sim->philo[i].tid, NULL, &start_routine, &sim->philo[i]);
		if (check != 0)
		{
			//error_handling
			return ;
		}
		i++;
	}
	i = 0;
	check = 0;
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
		printf("Error. Please enter:\nnb_of_philo, time_to_die, time_to_eat, time to_sleep, nb_of_meal(optionnal)\n");
		return (EXIT_FAILURE);
	}
	if (!setup_dinner_table(argv, &sim))
	{
//		terminate_simulation(&sim);
		return (EXIT_FAILURE);
	}
	launch_simulation(&sim);
	return (EXIT_SUCCESS);
}
