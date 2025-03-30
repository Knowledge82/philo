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

	if (philos[0].rules->num_times_to_eat < 0)
		return (0);
	rules = philos[0].rules;
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
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0)
		precise_sleep(1);  
	if (philo->rules->num_philos == 1)
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
                return (NULL);
            }
            pthread_mutex_unlock(&philo->rules->death_mutex);
            usleep(1000);	
		}
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
		// Tomar tenedores
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_status(philo, "has taken a fork");
		// Comer
		print_status(philo, GREEN"is eating"RESET);
		pthread_mutex_lock(&philo->meal_mutex);
		philo->last_meal_time = get_time_ms();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->meal_mutex);
		precise_sleep(philo->rules->time_to_eat);
		// Soltar tenedores
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		// Dormir
		print_status(philo, "is sleeping");
		precise_sleep(philo->rules->time_to_sleep);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_philosopher	*philos;
	int		i;

	philos = (t_philosopher *)arg;
	while (1)
	{
		i = 0;
		while (i < philos[0].rules->num_philos)
		{
			if (check_death(&philos[i]))
				return (NULL);
			i++;
			if (all_philos_ate_enough(philos))
			{
				pthread_mutex_lock(&philos[0].rules->death_mutex);
				printf(GREEN"All philosophers ate enough.\n"RESET);
				philos[0].rules->end_of_simulation = 1;
				pthread_mutex_unlock(&philos[0].rules->death_mutex);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}
