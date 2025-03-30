#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>

#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define RESET   "\033[0m"

typedef struct	s_rules t_rules;

typedef struct	s_philosopher
{
	int	id;
	int	left_fork_id;
	int	right_fork_id;
	int	meals_eaten;
	long long	time_last_meal;
	t_rules	*rules;
	pthread_t	thread_id;
} t_philosopher;

typedef struct	s_rules
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat;
	int	all_full;
	int	death;
	long long start_simulation;
	pthread_mutex_t eat_mutex;
	pthread_mutex_t death_mutex;
	pthread_mutex_t write_mutex;
	pthread_mutex_t *forks;
	t_philosopher *philosophers;
} t_rules;


//prots

int	write_error(char *str);
int init(t_rules *rules, char **argv);
long long	get_time_ms(void);
void	ft_sleep(long long duration, t_rules *rules);
int	ft_atoi(const char *str);
void	print_msg(t_rules *rules, int id, char *msg);
void *life_routine(void *arg);
int	simulation(t_rules *rules);

#endif
