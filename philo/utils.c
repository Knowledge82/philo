/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:28:59 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/09 16:01:32 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	int			i;
	long long	num;

	i = 0;
	num = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		printf("Error: only positive values allowed\n");
		return (-1);
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		if (num > 2147483647)
		{
			printf("Error: only values less then INT_MAX allowed\n");
			return (-1);
		}
		i++;
	}
	return ((int)num);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(long long time_ms)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time_ms)
		usleep(5);
}

void	print_status(t_philosopher *philo, char *status)
{
	long long	current_time;

	pthread_mutex_lock(&philo->rules->print_mutex);
	pthread_mutex_lock(&philo->rules->death_mutex);
	if (!philo->rules->end_of_simulation)
	{
		current_time = get_time_ms() - philo->rules->start_time;
		printf("%lld %d %s\n", current_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->rules->death_mutex);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

void	clean_simulation(t_rules *rules, t_philosopher *philos)
{
	int	i;

	if (philos)
	{
		i = 0;
		while (i < rules->num_philos)
		{
			pthread_mutex_destroy(&philos[i].meal_mutex);
			i++;
		}
		free(philos);
	}
	if (rules->forks)
	{
		i = 0;
		while (i < rules->num_philos)
		{
			pthread_mutex_destroy(&rules->forks[i].mutex);
			i++;
		}
		free(rules->forks);
	}
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
}
