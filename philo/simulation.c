/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:37:18 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/09 15:56:16 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (get_time_ms() - philo->last_meal_time > philo->rules->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		pthread_mutex_lock(&philo->rules->death_mutex);
		philo->rules->end_of_simulation = 1;
		pthread_mutex_unlock(&philo->rules->death_mutex);
		pthread_mutex_lock(&philo->rules->print_mutex);
		printf(RED"%lld %d died\n"RESET,
			get_time_ms() - philo->rules->start_time,
			philo->id);
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

int	all_philos_ate_enough(t_philosopher *philos)
{
	int			i;
	int			count;
	t_rules		*rules;

	rules = philos[0].rules;
	if (rules->num_times_to_eat < 0)
		return (0);
	pthread_mutex_lock(&rules->death_mutex);
	if (rules->end_of_simulation)
	{
		pthread_mutex_unlock(&rules->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&rules->death_mutex);
	count = 0;
	i = 0;
	while (i < rules->num_philos)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].meals_eaten >= rules->num_times_to_eat)
			count++;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	return (count == rules->num_philos);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->rules->num_philos == 1)
	{
		solo_philo(philo);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->rules->death_mutex);
		if (philo->rules->end_of_simulation)
		{
			pthread_mutex_unlock(&philo->rules->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->rules->death_mutex);
		print_status(philo, "is thinking");
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		sleep_philo(philo);
		usleep(10);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	int				i;
	t_philosopher	*philos;

	philos = (t_philosopher *)arg;
	while (1)
	{
		i = 0;
		while (i < philos[0].rules->num_philos)
		{
			if (check_death(&philos[i]))
				return (NULL);
			i++;
		}
		if (all_philos_ate_enough(philos))
		{
			pthread_mutex_lock(&philos[0].rules->death_mutex);
			printf(GREEN"All philosophers ate enough.\n"RESET);
			philos[0].rules->end_of_simulation = 1;
			pthread_mutex_unlock(&philos[0].rules->death_mutex);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
