/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/13 20:02:40 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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
	log_status(philo, "has taken a second fork");
	return (true);
}

void	release_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->nb - 1;
	right = philo->nb % philo->sim->rules.nb_of_philo;
	if (left > right)
		swap(&left, &right);
	pthread_mutex_unlock(&philo->sim->fork[left]);
	pthread_mutex_unlock(&philo->sim->fork[right]);
}

void	start_eating(t_philo *philo)
{
	philo->state = IS_EATING;
	log_status(philo, "is eating");
	usleep(philo->sim->rules.time_to_eat * 1000);
	gettimeofday(&philo->last_meal, NULL);
}

void	start_sleeping(t_philo *philo)
{
	philo->state = IS_SLEEPING;
	log_status(philo, "is sleeping");
	usleep(philo->sim->rules.time_to_sleep * 1000);
}

void	start_thinking(t_philo *philo)
{
	philo->state = IS_THINKING;
	log_status(philo, "is thinking");
	usleep(1000);
}

bool	someone_is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->death_check);
	if (philo->sim->is_dead)
	{
		pthread_mutex_unlock(&philo->sim->death_check);
		return (true);
	}
	pthread_mutex_unlock(&philo->sim->death_check);
	return (false);
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
		if (someone_is_dead(philo))
			break ;
		if (locks_two_forks_in_order(philo))
		{
			start_eating(philo);
			nb_of_meal++;
			release_forks(philo);
			if (nb_of_meal_reached(philo, nb_of_meal) == true)
				break ;
			start_sleeping(philo);
		}
		start_thinking(philo);
	}
	return (NULL);
}

void	monitoring(t_simulation *sim)
{
	int				i;
	long			timestamp;

	i = 0;
	while (1)
	{
		if (i >= sim->rules.nb_of_philo)
			i = 0;
		timestamp = get_timestamp_ms(sim);
		if (timestamp >= sim->rules.time_to_die)
		{
			pthread_mutex_lock(&sim->death_check);
			sim->is_dead = true;
			pthread_mutex_unlock(&sim->death_check);
		//	printf("%lu philo %i died\n", timestamp, i);
			log_status(&sim->philo[i], "died");
			return ;
		}
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
//		terminate_simulation(&sim);
		return (EXIT_FAILURE);
	}
	launch_simulation(&sim);
	return (EXIT_SUCCESS);
}
