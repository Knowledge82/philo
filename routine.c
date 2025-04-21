/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:16:37 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/09 15:20:50 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		usleep(1000);
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_status(philo, "has taken a fork");
	}
}

void	eat(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, GREEN"is eating"RESET);
	precise_sleep(philo->rules->time_to_eat);
}

void	put_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

void	sleep_philo(t_philosopher *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->rules->time_to_sleep);
}

void	solo_philo(t_philosopher *philo)
{
	print_status(philo, "is thinking");
	pthread_mutex_lock(&philo->left_fork->mutex);
	print_status(philo, "has taken a fork");
	while (1)
	{
		pthread_mutex_lock(&philo->rules->death_mutex);
		if (philo->rules->end_of_simulation)
		{
			pthread_mutex_unlock(&philo->rules->death_mutex);
			pthread_mutex_unlock(&philo->left_fork->mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->rules->death_mutex);
		usleep(1000);
	}
}
