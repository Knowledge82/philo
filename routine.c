#include "philo.h"

void	*life_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 1)
		usleep(1000);
	while(1)
	{
		if(check_simulation_end(philo->rules))
			return(NULL);	
		//берёт вилки
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->right_fork);
			ft_print_msg(philo, "is taking fork");	
			pthread_mutex_lock(philo->left_fork);
			ft_print_msg(philo, "is taking fork");	
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			ft_print_msg(philo, "is taking fork");
			pthread_mutex_lock(philo->right_fork);
			ft_print_msg(philo, "is taking fork");	
		}
		//обновить время последнего хавчика
		pthread_mutex_lock(&philo->rules->death_mutex);
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->rules->death_mutex);
			
		//ест
		ft_print_msg(philo, GREEN "is eating" RESET);
		ft_wait(philo->rules->time_to_eat, philo->rules);

		// увеличиваем счётчик приёмов пищи
		pthread_mutex_lock(&philo->rules->death_mutex);
		philo->meals_eaten++;

		if(philo->rules->must_eat > 0 && philo->meals_eaten == philo->rules->must_eat)
		{
			philo->rules->philos_full++;
			if(philo->rules->philos_full == philo->rules->num_philos)
			{
				ft_print_msg(philo, GREEN "Simulation end. All philosophers have eaten enough\n" RESET);
				philo->rules->simulation_end = 1;
			}
		}
		pthread_mutex_unlock(&philo->rules->death_mutex);

		//оставляет вилки
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (check_simulation_end(philo->rules))
			return (NULL);	
	
		// спит
		ft_print_msg(philo, "is sleeping");	
		ft_wait(philo->rules->time_to_sleep, philo->rules);	
		if (check_simulation_end(philo->rules))
			return (NULL);	

		// думает
		ft_print_msg(philo, "is thinking");
		usleep(100);	
	}
	return (NULL);
}
