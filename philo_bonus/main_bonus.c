/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:07:39 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/19 16:56:46 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	wait_for_philosophers(t_rules *rules, int *full_count)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	if (pid == -1)
		return (-1);
	if (WIFEXITED(status) && (WEXITSTATUS(status) == 1))
	{
		kill_them_all(rules);
		return (1);
	}
	else if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
	{
		(*full_count)++;
		if (*full_count == rules->num_philos)
		{
			sem_wait(rules->print_sem);
			printf(GREEN"All philosophers ate enough\n");
			sem_post(rules->print_sem);
			kill_them_all(rules);
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int			full_count;
	t_rules		rules;

	full_count = 0;
	memset(&rules, 0, sizeof(t_rules));
	if (check_args(argc, argv, &rules))
		return (1);
	if (init_simulation(&rules))
		return (1);
	if (start_simulation(&rules))
	{
		perror("Failed start simulation: ");
		clean_simulation(&rules);
		return (1);
	}
	while (1)
	{
		if (wait_for_philosophers(&rules, &full_count))
			break ;
		if (full_count == rules.num_philos)
			break ;
	}
	clean_simulation(&rules);
	return (0);
}
