#include "philo.h"

int	main(int argc, char **argv)
{
	int	i;
	t_rules	rules;
	t_philosopher	*philos;
	pthread_t	monitor_thread;

	memset(&rules, 0, sizeof(t_rules));
	if (check_args(argc, argv, &rules))
		return (1);
	if (init_simulation(&rules, &philos))
	{
		printf("Error: Could not initialize simulation\n");
		clean_simulation(&rules, philos);
		return (1);
	}
	if (create_simulation(philos, &monitor_thread))
	{
		printf("Error: Could not create simulation\n");
		clean_simulation(&rules, philos);
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < rules.num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	usleep(100);
	clean_simulation(&rules, philos);
	return (0);
}
