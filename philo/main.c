/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:26:10 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/09 17:59:24 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_simulation(t_rules *rules, t_philosopher **philos)
{
	int			i;
	pthread_t	monitor_thread;	

	if (init_simulation(rules, philos))
	{
		printf("Error: Could not initialize simulation\n");
		return (1);
	}
	if (create_simulation(*philos, &monitor_thread))
	{
		printf("Error: Could not create simulation\n");
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < rules->num_philos)
	{
		pthread_join((*philos)[i].thread, NULL);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules			rules;
	t_philosopher	*philos;

	if (check_args(argc, argv, &rules))
		return (1);
	if (start_simulation(&rules, &philos))
	{
		clean_simulation(&rules, philos);
		return (1);
	}
	clean_simulation(&rules, philos);
	return (0);
}
