#include "philo.h"

void	free_resources(t_rules *rules)
{
	int	i;

	// free philos
	if (rules->philosophers)
		free(rules->philosophers);

	// destr mutex forks
	if (rules->forks)
	{
		i = 0;
		while(i < rules->num_philos)
		{
			pthread_mutex_destroy(&rules->forks[i]);
			i++;
		}
		free(rules->forks);
	}
	// destr sys mutex
	pthread_mutex_destroy(&rules->eat_mutex);
	pthread_mutex_destroy(&rules->write_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
}

int	main(int argc, char **argv)
{
	t_rules	rules;
	int	result;

	if (argc != 5 && argc != 6)
	{
		write_error("Incorrect number of arguments");
		printf("Usage: ./%s <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (1);
	}
	memset(&rules, 0, sizeof(t_rules));
	result = init(&rules, argv);

	if (result != 0)
	{
		write_error("Initialization failed");
		free_resources(&rules);
		return (1);
	}
	result = simulation(&rules);
	free_resources(&rules);
	return (result);
}
