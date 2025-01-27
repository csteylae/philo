/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/01/27 17:40:13 by csteylae         ###   ########.fr       */
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


bool	someone_is_dead(t_philo *philo)
{
	bool				res;
	struct timeval_t	current_time;

	pthread_mutex_lock(&philo->sim->death_check);
	if (philo->sim->is_dead == true)
	{
		pthread_mutex_unlock(&philo->sim->death_check);
		return (true);
	}
}

long long	convert_into_ms(struct timeval current_time, struct timeval starting_time)
{
	long long	timestamp_in_ms;
	//should now also convert starting time into ms and then substract it to get the time since the simu begin to better tracking of the philo

	timestamp_in_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (timestamp_in_ms);
}

void	takes_two_fork(t_philo *philo)
{
	int				left;
	int				right;
	struct timeval	current_time;
	int				nb_of_meal;

	left = philo->nb - 1;
	right = philo->nb;
	nb_of_meal = 0;
	if (right == 0)
	{
		left = philo->sim->rules.nb_of_philo - 1;
	}
	while (true)
	{
		if (someone_is_dead(philo))
			break ;

		if (philo->sim->rules.nb_of_meal != UNLIMITED_MEAL && nb_of_meal == philo->sim->rules.nb_of_meal)
			break ;

		nb_of_meal++;

		if (pthread_mutex_lock(&philo->sim->fork[left]) == 0
			&& pthread_mutex_lock(&philo->sim->fork[right]) == 0)
		{
			if (gettimeofday(&current_time, NULL) != SUCCESS)
				return ;
			pthread_mutex_lock(&philo->sim->write_msg);
			printf("%lu %i has taken a fork\n", convert_into_ms(current_time), philo->nb); 
			pthread_mutex_unlock(&philo->sim->write_msg);

			if (gettimeofday(&current_time, NULL) != SUCCESS)
				return ; //ERROR
			start_eating_time = current_time;
			philo->last_meal = current_time;
			pthread_mutex_lock(&philo->sim->write_msg);
			printf("%lu %i is eating\n", convert_into_ms(current_time), philo->nb); 
			pthread_mutex_unlock(&philo->sim->write_msg);
			{
				usleep(philo->sim->rules.time_to_eat / 1000);
			}
		//sto eating,free the forks	
			pthread_mutex_unlock(&philo->sim->fork[left]);
			pthread_mutex_unlock(&philo->sim->fork[right]);
		//start to sleep
			gettimeofday(&current_time, NULL);
			pthread_mutex_lock(&philo->sim->write_msg);
			printf("%lu %i is sleeping \n", convert_into_msg(current_time), philo->nb); 
			pthread_mutex_unlock(&philo->sim->write_msg);
			usleep(philo->sim->rules.time_to_sleep / 1000);

		//end of sleep, philo starts thinking and look for eating
			pthread_mutex_lock(&philo->sim->write_msg);
			printf("%lu %i is thinking \n", convert_into_ms(current_time), philo->nb); 
			pthread_mutex_unlock(&philo->sim->write_msg);
		}
		else
			continue ;
	}
	return ;
}

void	*start_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	current_time;

	philo = arg;
	if (gettimeofday(&current_time, NULL) != SUCCESS)
	{
		printf("Sycall error. End of simulation\n");
		return (NULL);
	}
	philo->last_meal = current_time;
	if (philo->nb % 2 == 0)
	{
		usleep(1000);
	}
	takes_two_fork(philo);
//		start_eating();
//		release_forks();
//		start_sleeping();
//		start_thinking();
//		nb_of_meal++;
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
	if (gettimeofday(&sim->starting_time, NULL) != SUCCESS)
	{
		printf("syscall error\n");
		return ;
	}
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
