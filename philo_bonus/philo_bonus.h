/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:27:13 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/19 17:01:44 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# define GREEN	"\033[0;32m"
# define RED	"\033[0;31m"
# define RESET	"\033[0m"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <signal.h>
# include <string.h>

typedef struct s_rules
{
	int			num_philos;
	int			death;
	int			all_philos_full;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	long long	num_times_to_eat;
	long long	start_time;
	sem_t		*forks_sem;
	sem_t		*print_sem;
	sem_t		*death_sem;
	sem_t		*meal_sem;
	pid_t		*pids;
}	t_rules;

typedef struct s_philosopher
{
	int			id;
	int			meals_eaten;
	long long	last_meal_time;
	pthread_t	monitor_thread;
	t_rules		*rules;
}	t_philosopher;

// checks_bonus.c
int			check_args(int argc, char **argv, t_rules *rules);

// init_bonus.c
int			init_semaphores(t_rules *rules);
int			init_simulation(t_rules *rules);

// utils_bonus.c
int			ft_atoi(char *str);
long long	get_time_ms(void);
void		precise_sleep(long long time_ms);
void		print_status(t_philosopher *philo, char *status);
void		clean_semaphores(t_rules *rules);

// simulation_bonus.c
int			start_simulation(t_rules *rules);
void		*monitor_death(void *arg);
void		philosopher_routine(t_philosopher *philo);
void		clean_simulation(t_rules *rules);
void		kill_them_all(t_rules *rules);

// routine_bonus.c
int			eat(t_philosopher *philo);
void		solo_philo(t_philosopher *philo);
void		take_forks(t_philosopher *philo);
void		put_forks(t_philosopher *philo);

// main.c
int			wait_for_philosophers(t_rules *rules, int *full_count);
#endif
