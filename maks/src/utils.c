/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarinov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 13:11:14 by mmarinov          #+#    #+#             */
/*   Updated: 2025/02/18 11:40:02 by mmarinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	error_exit(const char *error)
{
	printf(RED"%s\n"RES, error);
}

void	ft_prints(t_dta *dta, int id, const char *acts)
{
	pthread_mutex_lock(&dta->write_lock);
	pthread_mutex_lock(&dta->dead_lock);
	if (!dta->death)
		printf(MAG "%lld %d %s\n" RES, time_now() - dta->start_time, id, acts);
	pthread_mutex_unlock(&dta->dead_lock);
	pthread_mutex_unlock(&dta->write_lock);
}

long long	time_now(void)
{
	struct timeval	now;
	long long		mlsec;

	gettimeofday(&now, NULL);
	mlsec = ((now.tv_sec * 1000) + (now.tv_usec / 1000));
	return (mlsec);
}

int	ft_usleep(long long time)
{
	long long	elapsed;
	long long	start_time;

	start_time = time_now();
	while (1)
	{
		elapsed = time_now() - start_time;
		if (elapsed >= time)
			break ;
		usleep(500);
	}
	return (1);
}

void	free_resources(t_dta *dta)
{
	int	i;

	i = 0;
	while (i < dta->n_filos)
	{
		pthread_mutex_destroy(&dta->forks[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&dta->write_lock);
	pthread_mutex_destroy(&dta->dead_lock);
	pthread_mutex_destroy(&dta->meal_lock);
	free(dta->filos);
	free(dta->forks);
}
