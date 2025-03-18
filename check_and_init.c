/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_and_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:48:27 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/03/18 13:39:01 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_arg(char *str)
{
	int i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[0] == '-' || str[0] == '+')
		return (0);
	while (str[i])
	{
		if (!isdigit(str[i]))
		return (0);
		i++;
	}
	return (1);
}

int	check_args(int argc, char **argv)
{
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
        return (0);
}

int     init_rules(t_rules *rules, int argc, char **argv)
{
	memset(rules, 0, sizeof(t_rules));
        rules->num_philos = atoi(argv[1]);
        rules->time_to_die = atoi(argv[2]);
        rules->time_to_eat = atoi(argv[3]);
        rules->time_to_sleep = atoi(argv[4]);
        rules->start_time = get_time();
        if (argc == 6)
                rules->must_eat = atoi(argv[5]);
        else
                rules->must_eat = -1;
        if(rules->num_philos <= 0)
        {
                error_message("Error: number of philosophers must be great than 0\n");
                return (1);
        }
        return (0);
}

int     init_mutexes(t_rules *rules, pthread_mutex_t *forks)
{
        int     i;

        pthread_mutex_init(&rules->write_mutex, NULL);
        pthread_mutex_init(&rules->death_mutex, NULL);
        i = 0;
        while (i < rules->num_philos)
        {
                pthread_mutex_init(&forks[i], NULL);
                i++;
        }
        return (0);
}

int     init_philosophers(t_philosopher **philos, pthread_mutex_t *forks, t_rules *rules)
{
        int     i;

        *philos = malloc(sizeof(t_philosopher) * rules->num_philos);
        if (!*philos)
        {
                error_message("Memory allocation failed\n");
                return(1);
        }
        i = 0;
        while (i < rules->num_philos)
        {
                (*philos)[i].id = i + 1;
                (*philos)[i].left_fork = &forks[i];
                (*philos)[i].right_fork = &forks[(i + 1) % rules->num_philos];
                (*philos)[i].meals_eaten = 0;
                (*philos)[i].last_meal_time = get_time();
                (*philos)[i].rules = rules;
                i++;
        }
        return (0);
}

int     create_philosophers_threads(t_philosopher *philos)
{
        int     i;

        i = 0;
        while(i < philos->rules->num_philos)
        {
                if (pthread_create(&philos[i].thread, NULL, life_routine, &philos[i]) != 0)
                {
                        error_message("Failed to create thread for philosopher\n");
                        return (1);
                }
                i++;
        }
        return(0);
}

int     create_monitor_thread(pthread_t *monitor_thread, t_philosopher *philos)
{
	if (!monitor_thread)
	{
		error_message("Monitor thread pointer is NULL\n");
		return (1);
	}
        if(pthread_create(monitor_thread, NULL, monitor_routine, philos) != 0)
        {
                error_message("Failed to create monitor thread\n");
                return (1);
        }
        return (0);
}

