/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:53:21 by csteylae          #+#    #+#             */
/*   Updated: 2025/01/27 16:15:34 by csteylae         ###   ########.fr       */
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
	pthread_t					tid;
	int							nb;
	int							state;
	suseconds_t					last_meal_usec;
	struct s_simulation			*sim;
}	t_philo;

typedef struct s_simulation
{
	t_rules			rules; //no malloc
	t_philo			*philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	write_msg;
	pthread_mutex_t	death_check;
	bool			is_dead;
}	t_simulation;

bool	get_rules(char **argv, t_rules *r);
bool	setup_dinner_table(char **argv, t_simulation *sim);
int		ft_strlen(char *str);

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
