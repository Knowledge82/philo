/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarinov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:33:46 by mmarinov          #+#    #+#             */
/*   Updated: 2025/02/18 15:58:34 by mmarinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	check_args(int ac, char **av)
{
	int	i;

	if (!check_int_count(ac))
		return (0);
	if (!check_num_filo(av[1]))
		return (0);
	i = 1;
	while (i < ac)
	{
		if (!check_params(av[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	start(int ac, char **av, t_dta *dta)
{
	if (!check_args(ac, av))
		return (0);
	dta->n_filos = ft_atoi(av[1]);
	if (dta->n_filos == -1)
		return (0);
	dta->tto_die = ft_atoi(av[2]);
	if (dta->tto_die == -1)
		return (0);
	dta->tto_eat = ft_atoi(av[3]);
	if (dta->tto_eat == -1)
		return (0);
	dta->tto_sleep = ft_atoi(av[4]);
	if (dta->tto_sleep == -1)
		return (0);
	if (ac == 6)
	{
		dta->n_meals = ft_atoi(av[5]);
		if (dta->n_meals == -1)
			return (0);
	}
	else
		dta->n_meals = 0;
	return (1);
}

static int	create_threads(t_dta *dta, pthread_t *monitor_thread)
{
	int	i;

	i = 0;
	while (i < dta->n_filos)
	{
		if (pthread_create(&dta->filos[i].thread, NULL,
				lifecycle, (void *)&dta->filos[i]) != 0)
		{
			error_exit("Error creating thread\n");
			return (1);
		}
		i++;
	}
	if (pthread_create(monitor_thread, NULL, monitor, (void *)dta) != 0)
	{
		error_exit("Error creating monitor thread\n");
		return (1);
	}
	return (0);
}

static void	join_threads(t_dta *dta, pthread_t monitor_thread)
{
	int	i;

	i = 0;
	while (i < dta->n_filos)
	{
		pthread_join(dta->filos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}

int	main(int ac, char **av)
{
	t_dta		*dta;
	pthread_t	monitor_thread;

	dta = malloc(sizeof(t_dta));
	if (!dta)
		return (1);
	if (!start(ac, av, dta))
	{
		free(dta);
		return (1);
	}
	init_dta(dta);
	if (init_filos(dta))
		return (1);
	create_threads(dta, &monitor_thread);
	join_threads(dta, monitor_thread);
	free_resources(dta);
	free(dta);
	return (0);
}
