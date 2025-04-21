/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:37:21 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/18 18:18:39 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_semaphores(t_rules *rules)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/death");
	sem_unlink("/meal");
	rules->forks_sem = sem_open("/forks", O_CREAT, 0644, rules->num_philos);
	rules->print_sem = sem_open("/print", O_CREAT, 0644, 1);
	rules->death_sem = sem_open("/death", O_CREAT, 0644, 1);
	rules->meal_sem = sem_open("/meal", O_CREAT, 0644, 1);
	if (rules->forks_sem == SEM_FAILED || rules->print_sem == SEM_FAILED
		|| rules->death_sem == SEM_FAILED || rules->meal_sem == SEM_FAILED)
	{
		perror("Failed init semaphores");
		return (1);
	}
	return (0);
}

int	init_simulation(t_rules *rules)
{
	if (init_semaphores(rules))
		return (1);
	rules->pids = malloc(sizeof(pid_t) * rules->num_philos);
	if (!rules->pids)
	{
		perror("Failed PID memory allocation");
		return (1);
	}
	return (0);
}
