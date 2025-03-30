#include "philo.h"

void	ft_print_msg(t_philosopher *philo, const char *msg)
{
	long long	time;

	time = get_time() - philo->rules->start_time;
	pthread_mutex_lock(&philo->rules->write_mutex);
	printf("%lld %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->rules->write_mutex);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while(str[len])
		len++;
	return (len);
}

void	error_message(char *text)
{
	if (text)
		write(2, text, ft_strlen(text));
	else
		write(2, "Error: NULL message\n", 20);
}

long long	get_time(void)
{
	struct	timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_simulation_end(t_rules *rules)
{
	int	end;
	pthread_mutex_lock(&rules->death_mutex);
	end = rules->simulation_end;
	pthread_mutex_unlock(&rules->death_mutex);
	return(end);
}

void	check_death(t_philosopher *philo)
{
	long long	current_time;
	long long	time_since_last_meal;

	current_time = get_time();

	pthread_mutex_lock(&philo->rules->death_mutex);
	time_since_last_meal = current_time - philo->last_meal_time;
	
	if(time_since_last_meal >= philo->rules->time_to_die && !philo->rules->simulation_end)
	{
		philo->rules->simulation_end = 1;
		pthread_mutex_unlock(&philo->rules->death_mutex);
		
		pthread_mutex_lock(&philo->rules->write_mutex);
		printf(RED "%lld %d died 💀\n" RESET, current_time - philo->rules->start_time, philo->id);
		pthread_mutex_unlock(&philo->rules->write_mutex);
		return;
	}
	pthread_mutex_unlock(&philo->rules->death_mutex);
}

int	check_meals(t_philosopher *philo)
{
	int	is_full;
	int	i;

	i = 0;
	is_full = 0;
	while(i < )
	pthread_mutex_lock(&philo->rules->death_mutex);
	if (philo->rules->must_eat > 0 && philo->meals_eaten >= philo->rules->must_eat)
		is_full = 1;
	pthread_mutex_unlock(&philo->rules->death_mutex);
	return(is_full);
}

void    *monitor_routine(void *arg)
{
    t_philosopher   *philos;
	int	i;
	int	full_philos;

	philos = (t_philosopher *)arg;

	while(1)
    {
		if(check_simulation_end(philos[0].rules))
			return(NULL);
		/*pthread_mutex_lock(&philos[0].rules->death_mutex);
		if(philos[0].rules->simulation_end)
		{
			pthread_mutex_unlock(&philos[0].rules->death_mutex);
			printf("DEBUG: Simulation should end now!\n");
			return(NULL);
		}
		pthread_mutex_unlock(&philos[0].rules->death_mutex);
		*/
		// обход всех философов
		i = 0;
		full_philos = 0;
		while(i < philos[0].rules->num_philos)
		{
			check_death(&philos[i]);
			if(check_simulation_end(philos[0].rules))
				return(NULL);
			if(check_meals(&philos[i]))
				full_philos++;
			i++;
		}
		if(full_philos == philos[0].rules->num_philos && philos[0].rules->must_eat > 0)
		{
			pthread_mutex_lock(&philos[0].rules->death_mutex);
			philos[0].rules->simulation_end = 1;
			pthread_mutex_unlock(&philos[0].rules->death_mutex);

			pthread_mutex_lock(&philos[0].rules->write_mutex);
			printf(GREEN "DEBUG: FROM MONITOR\n" RESET);
			printf(GREEN "All philosophers have eaten enough. Simulation ending.\n" RESET);
			pthread_mutex_unlock(&philos[0].rules->write_mutex);
			printf("DEBUG: Simulation should end now!\n");
			return(NULL);
		}
		usleep(1000);
	}
    return(NULL);
}

void	ft_wait(int duration, t_rules *rules)
{
	long long	start;
	start = get_time();
	while(get_time() - start < duration)
	{
		if(check_simulation_end(rules))
			return;
		usleep(1000);
	}
}


