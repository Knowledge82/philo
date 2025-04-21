/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:23:49 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/09 13:52:21 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(t_rules *rules, t_philosopher **philos)
{
	int	i;

	*philos = malloc(sizeof(t_philosopher) * rules->num_philos);
	if (!(*philos))
	{
		printf("Error: failed memory allocation for philosophers.\n");
		return (1);
	}
	i = 0;
	while (i < rules->num_philos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].last_meal_time = get_time_ms();
		(*philos)[i].rules = rules;
		(*philos)[i].left_fork = &rules->forks[i];
		(*philos)[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];
		if (pthread_mutex_init(&(*philos)[i].meal_mutex, NULL) != 0)
		{
			printf("Error: failed init meal_mutex.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_rules(t_rules *rules)
{
	int	i;

	rules->end_of_simulation = 0;
	rules->forks = malloc(sizeof(t_fork) * rules->num_philos);
	if (!rules->forks)
	{
		printf("Error: failed memory allocation for rules\n");
		return (1);
	}
	i = 0;
	while (i < rules->num_philos)
	{
		if (pthread_mutex_init(&rules->forks[i].mutex, NULL) != 0)
		{
			printf("Error: failed init forks\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_simulation(t_rules *rules, t_philosopher **philos)
{
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
	{
		printf("Error: failed init print_mutex\n");
		return (1);
	}
	if (pthread_mutex_init(&rules->death_mutex, NULL) != 0)
	{
		printf("Error: failed init death_mutex\n");
		return (1);
	}
	if (init_rules(rules))
		return (1);
	if (init_philos(rules, philos))
		return (1);
	return (0);
}

int	create_simulation(t_philosopher *philos, pthread_t *monitor_thread)
{
	int		i;
	t_rules	*rules;

	rules = philos[0].rules;
	rules->start_time = get_time_ms();
	i = 0;
	while (i < rules->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, routine, &philos[i]) != 0)
		{
			printf("Error: failed to create philosophers threads\n");
			return (1);
		}
		i++;
	}
	if (pthread_create(monitor_thread, NULL, monitor_routine, philos) != 0)
	{
		printf("Error: failed to create monitor thread\n");
		return (1);
	}
	return (0);
}
