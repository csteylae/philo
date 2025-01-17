/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:08:47 by csteylae          #+#    #+#             */
/*   Updated: 2025/01/17 13:17:48 by csteylae         ###   ########.fr       */
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

int	main(int ac, char **argv)
{
	t_rules	rules;

	if (ac != 5 && ac != 6)
	{
		printf("Error. Please enter:\nnb_of_philo, time_to_die, time_to_eat, time to_sleep, nb_of_meal(optionnal)\n");
		return (EXIT_FAILURE);
	}
	if (get_rules(argv, &rules) == FAIL)
		return (EXIT_FAILURE);
	print_rules(rules);
		return (EXIT_FAILURE);
//	init_philo();
//	launch_routine();
//	terminate_simulation();
	return (EXIT_SUCCESS);
}
