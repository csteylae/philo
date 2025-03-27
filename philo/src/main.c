/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csteylae <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:16 by csteylae          #+#    #+#             */
/*   Updated: 2025/03/27 15:52:59 by csteylae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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
