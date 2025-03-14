/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:53:21 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/14 19:33:18 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>

struct	s_simulation;

typedef struct s_rules
{
	int	nb_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_of_meal;
}	t_rules;

typedef struct s_philo
{
	pthread_t			tid;
	int					nb;
	int					state;
	int					nb_of_meal;
	long				last_meal;
	struct s_simulation	*sim;
}	t_philo;

typedef struct s_simulation
{
	struct timeval	starting_time;
	t_rules			rules;
	t_philo			*philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	write_msg;
	pthread_mutex_t	death_check;
	pthread_mutex_t	run_check;
	bool			is_dead;
	bool			is_running;
}	t_simulation;

bool	get_rules(char **argv, t_rules *r);
void	init_last_meal(t_philo *philo);
bool	setup_dinner_table(char **argv, t_simulation *sim);
int		ft_strlen(char *str);
long	get_timestamp_ms(t_simulation *sim);
void	log_status(t_philo *philo, char *str);
void	terminate_simulation(t_simulation *sim);
void	ft_usleep(t_philo *philo, long time_is_ms);
bool	locks_two_forks_in_order(t_philo *philo);
void	release_forks(t_philo *philo);
void	start_eating(t_philo *philo);
void	start_sleeping(t_philo *philo);
void	monitoring(t_simulation *sim);
bool	is_running(t_philo *philo);

# define FAIL -1
# define SUCCESS 0
# define UNLIMITED_MEAL 0
# define HUNGRY 0
# define IS_EATING 1
# define IS_THINKING 2
# define IS_SLEEPING 3
# define NO_MEAL_YET -1
# define MUTEX_INIT_ERROR "Error when pthread_init_mutex"
# define MALLOC_ERROR "Error cannot malloc"

#endif
