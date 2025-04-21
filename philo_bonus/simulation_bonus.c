/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:17:15 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/19 17:13:48 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor_death(void *arg)
{
	t_philosopher	*philo;
	long long		current_time;

	philo = (t_philosopher *)arg;
	while (1)
	{
		sem_wait(philo->rules->meal_sem);
		current_time = get_time_ms();
		if ((current_time - philo->last_meal_time) >= philo->rules->time_to_die)
		{
			sem_post(philo->rules->meal_sem);
			print_status(philo, RED"died"RESET);
			sem_post(philo->rules->death_sem);
			free(philo);
			exit (1);
		}
		sem_post(philo->rules->meal_sem);
		precise_sleep(5);
	}
	return (NULL);
}

void	philosopher_routine(t_philosopher *philo)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, monitor_death, philo);
	pthread_detach(monitor);
	if (philo->rules->num_philos == 1)
	{
		solo_philo(philo);
		exit (0);
	}
	if (philo->id % 2 == 0)
		precise_sleep(10);
	while (1)
	{
		take_forks(philo);
		if (eat(philo))
		{
			put_forks(philo);
			exit (0);
		}
		put_forks(philo);
		print_status(philo, "is sleeping");
		precise_sleep(philo->rules->time_to_sleep);
		print_status(philo, "is thinking");
	}
}

int	start_simulation(t_rules *rules)
{
	int				i;
	t_philosopher	*philo;

	rules->start_time = get_time_ms();
	i = 0;
	while (++i <= rules->num_philos)
	{
		rules->pids[i] = fork();
		if (rules->pids[i] < 0)
			return (1);
		else if (rules->pids[i] == 0)
		{
			philo = malloc(sizeof(t_philosopher));
			if (!philo)
				exit (1);
			philo->rules = rules;
			philo->id = i;
			philo->meals_eaten = 0;
			philo->last_meal_time = get_time_ms();
			philosopher_routine(philo);
			free(philo);
			exit(0);
		}
	}
	return (0);
}

void	kill_them_all(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philos)
	{
		if (rules->pids[i] > 0)
			kill(rules->pids[i], SIGTERM);
		i++;
	}
}

void	clean_simulation(t_rules *rules)
{
	int	i;

	kill_them_all(rules);
	i = 0;
	while (i < rules->num_philos)
	{
		if (rules->pids[i] > 0)
		{
			waitpid(rules->pids[i], NULL, 0);
		}
		i++;
	}
	clean_semaphores(rules);
	if (rules->pids)
	{
		free(rules->pids);
	}
}
