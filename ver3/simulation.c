#include "philo.h"

void	take_forks(t_philosopher *philo)
{
	
	t_rules	*rules;

	rules = philo->rules;
	pthread_mutex_lock(&rules->forks[philo->left_fork_id]);
	print_msg(rules, philo->id, "has taken a fork");
	pthread_mutex_lock(&rules->forks[philo->right_fork_id]);
	print_msg(rules, philo->id, "has taken a fork");
}


void	put_forks(t_philosopher *philo)
{
	t_rules *rules;

	rules = philo->rules;
	pthread_mutex_unlock(&rules->forks[philo->left_fork_id]);
	pthread_mutex_unlock(&rules->forks[philo->right_fork_id]);
}

void	eat(t_philosopher *philo)
{
	t_rules	*rules;

	rules = philo->rules;

	take_forks(philo);
	
	pthread_mutex_lock(&rules->eat_mutex);
	print_msg(rules, philo->id, GREEN"is eating"RESET);
	philo->time_last_meal = get_time_ms();
	pthread_mutex_unlock(&rules->eat_mutex);
	
	ft_sleep(rules->time_to_eat, rules);	
	philo->meals_eaten++;
	put_forks(philo);	
}

void	*life_routine(void *arg)
{
	
	t_philosopher	*philo;
	t_rules			*rules;

	philo = (t_philosopher *)arg;
	rules = philo->rules;
	if (philo->id % 2)
		usleep(15000);
	while (1)
	{
	/*	pthread_mutex_lock(&rules->death_mutex);
        if (rules->death || rules->all_full)
        {
            pthread_mutex_unlock(&rules->death_mutex);
            break;
        }
        pthread_mutex_unlock(&rules->death_mutex);
	*/	
		eat(philo);
	/*
		pthread_mutex_lock(&rules->death_mutex);
		if (rules->all_full)
		{
			pthread_mutex_unlock(&rules->death_mutex);
			break;
		}
        pthread_mutex_unlock(&rules->death_mutex);
	*/	
		print_msg(rules, philo->id, "is sleeping");
		ft_sleep(rules->time_to_sleep, rules);

		print_msg(rules, philo->id, "is thinking");
	}
	return (NULL);	
}

void	check_death(t_rules *rules)
{
	int	i;

	i = 0;
	while(i < rules->num_philos)
	{
       if(get_time_ms() - rules->philosophers[i].time_last_meal > rules->time_to_die)
	   {
			pthread_mutex_lock(&rules->death_mutex);
			if (!rules->death)
            {
                rules->death = 1;
                print_msg(rules, rules->philosophers[i].id, RED"died"RESET);
			}
			pthread_mutex_unlock(&rules->death_mutex);
			break ;
		}	   
		i++;
    }
}

void	check_eat(t_rules *rules)
{
	int	full;
	int	i;

	i = 0;
	full = 0;
	while(i < rules->num_philos)
	{
		pthread_mutex_lock(&rules->eat_mutex);
		if (rules->philosophers[i].meals_eaten >= rules->must_eat)
			full++;
		pthread_mutex_unlock(&rules->eat_mutex);
		i++;
	}
	if (full == rules->num_philos)
	{
		pthread_mutex_lock(&rules->death_mutex);
		printf(GREEN"All philosophers reaches meal limit. Simulation ending...\n"RESET);
		rules->all_full = 1;
		pthread_mutex_unlock(&rules->death_mutex);
		
	}
}

void	*monitor_routine(void *arg)
{
	t_rules	*rules;
	rules = (t_rules *)arg;
	while(1)
	{
		pthread_mutex_lock(&rules->death_mutex);
		if(rules->death || rules->all_full)
		{
			pthread_mutex_unlock(&rules->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&rules->death_mutex);

		check_death(rules);
		if(rules->must_eat != -1)
			check_eat(rules);
		usleep(1000);
	}
	return(NULL);
}
/*
void	*monitor_routine(void *arg)
{
	t_rules	*rules;
	int	i;
//	int	full_count;
	long long current_time;

	rules = (t_rules *)arg;
	while(1)
	{		
		pthread_mutex_lock(&rules->death_mutex);
		if(rules->death || rules->all_full)
		{
			printf("DEBUG(in monitor): monitor_routine detected termination condition.\n");
			pthread_mutex_unlock(&rules->death_mutex);
			break;
		}
		pthread_mutex_unlock(&rules->death_mutex);

		i = 0;
	//	full_count = 0;
		current_time = get_time_ms();	
		while(i < rules->num_philos)
		{
			pthread_mutex_lock(&rules->eat_mutex);
			
			//death
			if (current_time - rules->philosophers[i].time_last_meal > rules->time_to_die)
			{
				pthread_mutex_lock(&rules->death_mutex);
				if (!rules->death)
				{
					print_msg(rules, rules->philosophers[i].id, RED"died"RESET);
					rules->death = 1;
			//???	pthread_mutex_unlock(&rules->eat_mutex);
				//	return (NULL);
				}
				pthread_mutex_unlock(&rules->death_mutex);
				pthread_mutex_unlock(&rules->eat_mutex);
		//		usleep(100);
		//		break;
				return (NULL);
			}
			pthread_mutex_unlock(&rules->eat_mutex);
			i++;
		}
		//LIMIT EDY
		if(rules->must_eat != -1)
		{
			i = 0;
				while(i < rules->num_philos)
				{
					pthread_mutex_lock(&rules->eat_mutex);
					if(rules->philosophers[i].meals_eaten < rules->must_eat)
					{
						pthread_mutex_unlock(&rules->eat_mutex);
						break;
					}
					pthread_mutex_unlock(&rules->eat_mutex);
					i++;
				}

				if(i == rules->num_philos)
				{
					pthread_mutex_lock(&rules->death_mutex);
					rules->all_full = 1;
					pthread_mutex_unlock(&rules->death_mutex);
					break;
				}
			}
		usleep(1000);
	}
	return (NULL);
}
			///////////////////////////////////////////////////////
			//meal limit
			if (rules->must_eat != -1 && rules->philosophers[i].meals_eaten >= rules->must_eat)
				full_count++;
			i++;
			pthread_mutex_unlock(&rules->eat_mutex);
		}
		pthread_mutex_lock(&rules->death_mutex);
		if (full_count == rules->num_philos)
		{
			printf(GREEN"All philosophers reaches meal limit. Simulation ending...\n"RESET);
			rules->all_full = 1;
		}
		pthread_mutex_unlock(&rules->death_mutex);
		usleep(10);
	}
	
	return (NULL);
	*/
int	simulation(t_rules *rules)
{
	int	i;
	pthread_t	monitor;

	i = 0;
	rules->start_simulation = get_time_ms();

	while (i < rules->num_philos)
	{
		if (pthread_create(&rules->philosophers[i].thread_id, NULL, life_routine, &rules->philosophers[i]) != 0)
		{
			write_error("Thread creation failed");
			return (1);
		}
		rules->philosophers[i].time_last_meal = rules->start_simulation;
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, rules) != 0)
	{
		write_error("Monitor thread creation failed");
		return (1);
	}
	i = 0;
	while (i < rules->num_philos)
	{
		pthread_join(rules->philosophers[i].thread_id, NULL);
		i++;
	}
	printf("DEBUG: Simulation ended. All threads joined.\n");
	pthread_join(monitor, NULL);
	return (0);
}

