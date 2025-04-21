/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:28:42 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/19 15:37:34 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	solo_philo(t_philosopher *philo)
{
	print_status(philo, "is thinking");
	sem_wait(philo->rules->forks_sem);
	print_status(philo, "has taken a fork");
	sem_post(philo->rules->forks_sem);
	precise_sleep(philo->rules->time_to_die + 1);
	return ;
}

void	take_forks(t_philosopher *philo)
{
	sem_wait(philo->rules->forks_sem);
	print_status(philo, "has taken a fork");
	sem_wait(philo->rules->forks_sem);
	print_status(philo, "has taken a fork");
}

int	eat(t_philosopher *philo)
{
	sem_wait(philo->rules->meal_sem);
	philo->last_meal_time = get_time_ms();
	sem_post(philo->rules->meal_sem);
	print_status(philo, GREEN"is eating"RESET);
	philo->meals_eaten++;
	precise_sleep(philo->rules->time_to_eat);
	if (philo->rules->num_times_to_eat != -1
		&& philo->meals_eaten >= philo->rules->num_times_to_eat)
		return (1);
	return (0);
}

void	put_forks(t_philosopher *philo)
{
	sem_post(philo->rules->forks_sem);
	sem_post(philo->rules->forks_sem);
}
