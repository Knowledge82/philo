#include "philo.h"

void	*life_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while(1)
	{
		if(check_simulation_end(philo->rules))
		{
			printf("DEBUG: Philo %d exiting life_routine\n", philo->id);
			return(NULL);	
		//	break;
		}
		//берёт вилки
		if (philo->id % 2 == 1)
		{
			if (check_simulation_end(philo->rules))
				return (NULL);
			pthread_mutex_lock(philo->left_fork);
			ft_print_msg(philo, "is taking fork");	
			pthread_mutex_unlock(philo->left_fork);
			
			if (check_simulation_end(philo->rules))
				return (NULL);	
			pthread_mutex_lock(philo->right_fork);
		/*	{
				printf("DEBUG: Unlock and exiting life_routine after for 4ET\n");
				pthread_mutex_unlock(philo->left_fork);
				pthread_mutex_unlock(philo->right_fork);
			//	break;
			}*/
			ft_print_msg(philo, "is taking fork");	
		}
		else
		{
			ft_wait((100 + philo->id * 10), philo->rules);
			if(check_simulation_end(philo->rules))
				return (NULL);
			pthread_mutex_lock(philo->right_fork);
			/*if(check_simulation_end(philo->rules))
			{
				pthread_mutex_unlock(philo->right_fork);
				return (NULL);
			//	break;
			}*/
			ft_print_msg(philo, "is taking fork");
			pthread_mutex_lock(philo->left_fork);
			if (check_simulation_end(philo->rules))
			{
			//	printf("DEBUG: Unlock and exiting life_routine after for NE4ET\n");
				pthread_mutex_unlock(philo->right_fork);
				pthread_mutex_unlock(philo->left_fork);
				return (NULL);	
			//	break;
			}

			ft_print_msg(philo, "is taking fork");	
		}
		//обновить время последнего хавчика
		pthread_mutex_lock(&philo->rules->death_mutex);
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->rules->death_mutex);
			
		//ест
		ft_print_msg(philo, GREEN "is eating" RESET);
		ft_wait(philo->rules->time_to_eat, philo->rules);
		if (check_simulation_end(philo->rules))
		{
		//	printf("DEBUG: Unlock and exiting life_routine in EST, Philo %d\n", philo->id);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (NULL);	
		}


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

		if (check_simulation_end(philo->rules))
		{
			printf("DEBUG: Unlock and exiting life_routine\n");
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (NULL);	
		}

		//оставляет вилки
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	
		// спит
		ft_print_msg(philo, "is sleeping");	
		ft_wait(philo->rules->time_to_sleep, philo->rules);	
		if (check_simulation_end(philo->rules))
			return (NULL);	

		// думает
		ft_print_msg(philo, "is thinking");	
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (NULL);
}
