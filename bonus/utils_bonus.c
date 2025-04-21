/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:03:21 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/19 17:00:57 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

	sem_wait(philo->rules->print_sem);
	current_time = get_time_ms() - philo->rules->start_time;
	printf("%lld %d %s\n", current_time, philo->id, status);
	sem_post(philo->rules->print_sem);
}

void	clean_semaphores(t_rules *rules)
{
	sem_close(rules->forks_sem);
	sem_close(rules->print_sem);
	sem_close(rules->death_sem);
	sem_close(rules->meal_sem);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/death");
	sem_unlink("/meal");
}
