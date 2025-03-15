#include "philo.h"

int	main(int argc, char **argv)
{
	int	i;
	pthread_mutex_t	*forks;
	t_philosopher	*philos;
	t_rules	rules;
	pthread_t	monitor_thread;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (1);
	}
	if (!is_valid_arg(argv[1]) || !is_valid_arg(argv[2]) || !is_valid_arg(argv[3]) || !is_valid_arg(argv[4]) || (argc == 6 && !is_valid_arg(argv[5])))
	{
		printf("Error: all args must be positive integers.\n");
		return (1);
	}
	rules.num_philos = atoi(argv[1]);
	rules.time_to_die = atoi(argv[2]);
	rules.time_to_eat = atoi(argv[3]);
	rules.time_to_sleep = atoi(argv[4]);
	rules.simulation_end = 0;
	rules.philos_full = 0;
	rules.start_time = get_time();
	if (argc == 6)
		rules.must_eat = atoi(argv[5]);
	else
		rules.must_eat = -1;
	if(rules.num_philos <= 0)
	{
		printf("Error: number of philosophers must be great than 0\n");
		return (1);
	}
	forks = malloc(sizeof(pthread_mutex_t) * rules.num_philos);
	philos = malloc(sizeof(t_philosopher) * rules.num_philos);
	if (!forks || !philos)
	{
		free(forks);
		free(philos);
		printf("Mem alloc failed\n");
		return(1);
	}
	// INIT MUTEXs
	pthread_mutex_init(&rules.write_mutex, NULL);
	pthread_mutex_init(&rules.death_mutex, NULL);
	i = 0;
	while (i < rules.num_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	// Creation of philosophers
	i = 0;
	while (i < rules.num_philos)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % rules.num_philos];
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = get_time(); //иниц последнего приёма пищи текущим временем
		philos[i].rules = &rules;
		if (pthread_create(&philos[i].thread, NULL, life_routine, &philos[i]) != 0)
		{
			printf("Error: Failed to create thread for philosopher %d\n", philos[i].id);
			return (1);
		}
		i++;
	}
	// START MONITOR
	if (pthread_create(&monitor_thread, NULL, monitor_routine, philos) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (1);
	}

	while(1)
	{
		pthread_mutex_lock(&rules.death_mutex);
		if(rules.simulation_end)
		{
			pthread_mutex_unlock(&rules.death_mutex);
			break;
		}
		pthread_mutex_unlock(&rules.death_mutex);
		usleep(10);
	}
	//waiting all philos
	i = 0;
	while (i < rules.num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);

	//clears
	pthread_mutex_destroy(&rules.write_mutex);
	pthread_mutex_destroy(&rules.death_mutex);
	i = 0;
	while (i < rules.num_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
	free(philos);
	return (0);
}
