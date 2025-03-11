/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/11 21:40:50 by csteylae         ###   ########.fr       */
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
	struct timeval	current_time;

	timestamp_in_ms = get_timestamp_ms(philo->sim);
	gettimeofday(&current_time, NULL);
	if (pthread_mutex_lock(&philo->sim->write_msg))
		printf("%lu %i %s\n", timestamp_in_ms, philo->nb, str);
	pthread_mutex_unlock(&philo->sim->write_msg);
}
	
static void	init_last_meal(t_philo *philo)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(&philo->sim->death_check);
	philo->last_meal = current_time;
	pthread_mutex_unlock(&philo->sim->death_check);
}

static bool	nb_of_meal_reached(t_philo *philo, int nb_of_meal)
{
	if (philo->sim->rules.nb_of_meal != UNLIMITED_MEAL)
	{
		if (nb_of_meal == philo->sim->rules.nb_of_meal)
			return (true);
		return (false);
	}
	return (false);
}

static void	swap(int *left, int *right)
{
	int	tmp;

	tmp = *left;
	*left = *right;
	*right = tmp;
}

static bool	locks_two_forks_in_order(t_philo *philo)
{
	int	right;
	int	left;

	left = philo->nb - 1;
	right = philo->nb % philo->sim->rules.nb_of_philo;
	if (left > right)
		swap(&left, &right);
	if (pthread_mutex_lock(&philo->sim->fork[left]) != SUCCESS)
		return (false);
	log_status(philo, "has taken fork");
	if (pthread_mutex_lock(&philo->sim->fork[right]) != SUCCESS)
	{
		pthread_mutex_unlock(&philo->sim->fork[left]);
		return (false);
	}
	log_status(philo, "has taken fork");
	return (true);
}

void	*start_dinner(void *arg)
{
	t_philo			*philo;
	int				nb_of_meal;

	philo = arg;
	nb_of_meal = 0;
	init_last_meal(philo);
	if (philo->nb % 2 == 0)
	{
		usleep(1000);
	}
	while (1)//sim_is_running(philo))
	{
		if (locks_two_forks_in_order(philo))
		{
	//		start_eating();
	//		release_forks();
			if (nb_of_meal_reached(philo, nb_of_meal) == true)
				break ;
	//		start_sleeping();
	//		start_thinking();
	//		nb_of_meal++;
		}
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
		printf("syscall error\n");
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
//		terminate_simulation(&sim);
		return (EXIT_FAILURE);
	}
	launch_simulation(&sim);
	return (EXIT_SUCCESS);
}

