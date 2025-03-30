#include "philo.h"



int	check_args(int argc, char **argv, t_rules *rules)
{
	int	argum;

    if (argc != 5 && argc != 6)
	{
		printf("Usage: ./%s <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return (1);
	}
    rules->end_of_simulation = 0;
    argum = ft_atoi(argv[1]);
	if (argum < 1 || argum > 200)
	{
		printf("Error: invalid number of philosophers.\n");
		return (1);
	}
	rules->num_philos = argum;
	
	argum = ft_atoi(argv[2]);
	if (argum <= 0)
	{
		printf("Error: invalid time_to_die.\n");
		return (1);
	}

    rules->time_to_die = argum;
    
	argum = ft_atoi(argv[3]);
	if (argum <= 0)
	{
		printf("Error: invalid time_to_eat.\n");
		return (1);
	}
	rules->time_to_eat = argum;
    
	argum = ft_atoi(argv[4]);
	if (argum <= 0)
	{
		printf("Error: invalid time_to_sleep.\n");
		return (1);
	}
	rules->time_to_sleep = argum;
    
	if (argc == 6)
	{
		argum = ft_atoi(argv[5]);
		if (argum <= 0)
		{
			printf("Error: number_of_times_each_philosopher_must_eat must be > 0.\n");
			return (1);
		}
		rules->num_times_to_eat = argum;
	}
    else
	{
        rules->num_times_to_eat = -1;
	}
	return (0);
}
