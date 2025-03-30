#include "philo.h"

int	init_mutexes(t_rules *rules)
{
	int	i;
	
	i = rules->num_philos;
	while (--i >= 0)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL))
			return (1);
	}
	if (pthread_mutex_init(&rules->eat_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&rules->write_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&rules->death_mutex, NULL))
		return (1);
    return (0);
}

int	init_philosophers(t_rules *rules)
{
	int	i;

	i = rules->num_philos;
	while (--i >= 0)
    {
        rules->philosophers[i].id = i;
		rules->philosophers[i].left_fork_id = i;
		rules->philosophers[i].right_fork_id = (i + 1) % rules->num_philos;
        rules->philosophers[i].meals_eaten = 0;
        rules->philosophers[i].time_last_meal = 0;
        rules->philosophers[i].rules = rules;
    }
    return (0);
}

int	init_rules(t_rules *rules, char **argv)
{
    rules->num_philos = ft_atoi(argv[1]);
    rules->time_to_die = ft_atoi(argv[2]);
    rules->time_to_eat = ft_atoi(argv[3]);
    rules->time_to_sleep = ft_atoi(argv[4]);
    rules->start_simulation = get_time_ms();
	rules->death = 0;
	rules->all_full = 0;
    if (argv[5])
	{
        rules->must_eat = ft_atoi(argv[5]);
		if (rules->must_eat <= 0)
			return (1);
	}
    else
        rules->must_eat = -1;
    if(rules->num_philos <= 0)
    {
        write_error("Error: number of philosophers must be great than 0");
        return (1);
    }
    return (0);
}

int	init(t_rules *rules, char **argv)
{
	if(init_rules(rules, argv) != 0)
		return (1);
	rules->philosophers = malloc(sizeof(t_philosopher) * rules->num_philos);
	if(!rules->philosophers)
		return(1);
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philos);
	if(!rules->forks)
	{
		free(rules->philosophers);
		return (1);
	}
	if(init_mutexes(rules) != 0)
	{
		free(rules->philosophers);
		free(rules->forks);
		return (1);
	}
	if(init_philosophers(rules) != 0)
	{
		free(rules->philosophers);
		free(rules->forks);
		return (1);
	}
	return(0);
}
