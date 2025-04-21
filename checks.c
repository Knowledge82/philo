/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:30:13 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/10 14:05:44 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_usage(char *program_name)
{
	printf("Usage: %s <number_of_philosophers> "
		"<time_to_die> <time_to_eat> <time_to_sleep> "
		"[number_of_times_each_philosopher_must_eat]\n", program_name);
}

static int	check_positive(int num, char *msg)
{
	if (num <= 0)
	{
		printf("Error: %s must be > 0\n", msg);
		return (1);
	}
	return (0);
}

static int	check_args_are_num(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error: all arguments must be positive integers\n");
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static int	check_and_assign(int argc, char **argv, t_rules *rules)
{
	rules->num_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->num_times_to_eat = -1;
	if (argc == 6)
	{
		rules->num_times_to_eat = ft_atoi(argv[5]);
		if (check_positive(rules->num_times_to_eat, "num times eat"))
			return (1);
	}
	if (rules->num_philos < 1 || rules->num_philos > 200)
	{
		printf("Error: invalid number of philosophers\n");
		return (1);
	}
	if (check_positive(rules->time_to_die, "time to die")
		|| check_positive(rules->time_to_eat, "time to eat")
		|| check_positive(rules->time_to_sleep, "time to sleep"))
		return (1);
	return (0);
}

int	check_args(int argc, char **argv, t_rules *rules)
{
	if (argc != 5 && argc != 6)
	{
		print_usage(argv[0]);
		return (1);
	}
	if (check_args_are_num(argc, argv))
		return (1);
	if (check_and_assign(argc, argv, rules))
		return (1);
	return (0);
}
