/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:53:21 by csteylae          #+#    #+#             */
/*   Updated: 2025/01/17 13:17:02 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct	s_rules
{
	int	nb_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_of_meal;
}	t_rules;

int	get_rules(char **argv, t_rules *r);

#define FAIL -1
#define SUCCESS 0
#define UNLIMITED_MEAL 0;
