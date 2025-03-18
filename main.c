/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:05:12 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/03/18 13:59:12 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_for_threads(t_philosopher *philos, pthread_t monitor_thread, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}

int	main(int argc, char **argv)
{
	int	i;
	t_rules	rules;
	t_philosopher	*philos;
	pthread_mutex_t	*forks;
	pthread_t	monitor_thread;

	forks = NULL;
	philos = NULL;

	if(check_args(argc, argv))
		return(1);
	if(init_rules(&rules, argc, argv))
		return(1);
	forks = malloc(sizeof(pthread_mutex_t) * rules.num_philos);
        	if (!forks)
        	{
                	error_message("Memory allocation failed\n");
                	return(1);
        	}
	if(init_mutexes(&rules, forks))
		return(1);
	if (pthread_mutex_init(&rules.death_mutex, NULL) != 0)
{
	printf("Error: Failed to initialize death_mutex\n");
	return (1);
}

	if(init_philosophers(&philos, forks, &rules))
		return(1);
	if(create_philosophers_threads(philos))
		return(1);
	if(create_monitor_thread(&monitor_thread, philos))
		return(1);
	while(!rules.simulation_end)
	{
		pthread_mutex_lock(&rules.death_mutex);
		if(rules.simulation_end)
		{
			pthread_mutex_unlock(&rules.death_mutex);
			break;
		}
		pthread_mutex_unlock(&rules.death_mutex);
		usleep(10);
	}
	if(!philos)
	{
		error_message("Philosophers array is NULL\n");
		return(1);
	}
	wait_for_threads(philos, monitor_thread, &rules);

	//clears
	
	pthread_mutex_destroy(&rules.write_mutex);
	pthread_mutex_destroy(&rules.death_mutex);
	i = 0;
	while (i < rules.num_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
	free(philos);
	return (0);
}
