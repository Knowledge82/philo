/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarsuye <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:53:21 by vdarsuye          #+#    #+#             */
/*   Updated: 2025/04/10 15:28:59 by vdarsuye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define GREEN	"\033[0;32m"
# define RED	"\033[0;31m"
# define RESET	"\033[0m"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
//# include <string.h>

typedef struct s_fork
{
	pthread_mutex_t		mutex;
}	t_fork;

typedef struct s_rules
{
	t_fork				*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
	int					num_philos;
	int					end_of_simulation;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	long long			num_times_to_eat;
	long long			start_time;
}	t_rules;

typedef struct s_philosopher
{
	pthread_t			thread;
	pthread_mutex_t		meal_mutex;
	int					id;
	int					meals_eaten;
	long long			last_meal_time;
	t_fork				*left_fork;
	t_fork				*right_fork;
	t_rules				*rules;
}	t_philosopher;

// utils.c
int			ft_atoi(char *str);
long long	get_time_ms(void);
void		precise_sleep(long long time_ms);
void		clean_simulation(t_rules *rules, t_philosopher *philos);
void		print_status(t_philosopher *philo, char *status);

// checks.c
int			check_args(int argc, char **argv, t_rules *rules);

// init.c
int			init_simulation(t_rules *rules, t_philosopher **philos);
int			init_philos(t_rules *rules, t_philosopher **philos);
int			init_rules(t_rules *rules);
int			create_simulation(t_philosopher *philos, pthread_t *monitor_thread);

// simulation.c
int			check_death(t_philosopher *philo);
int			all_philos_ate_enough(t_philosopher *philos);
void		*routine(void *arg);
void		*monitor_routine(void *arg);

// routine.c
void		take_forks(t_philosopher *philo);
void		eat(t_philosopher *philo);
void		put_forks(t_philosopher *philo);
void		sleep_philo(t_philosopher *philo);
void		solo_philo(t_philosopher *philo);

#endif
