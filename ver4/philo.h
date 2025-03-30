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
# include <string.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_rules
{
	int			num_philos;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	long long	start_time;
	int			num_times_to_eat;
	int			end_of_simulation;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	t_fork		*forks;
}	t_rules;

typedef struct	s_philosopher
{
    int	id;
    int	meals_eaten;
    long long	last_meal_time;
    pthread_t	thread;
    pthread_mutex_t	meal_mutex;
    t_fork	*left_fork;
    t_fork	*right_fork;
    t_rules	*rules;
} t_philosopher;

// utils.c
int	ft_atoi(char *str);
long long	get_time_ms(void);
void	precise_sleep(long long time_ms);
void	clean_simulation(t_rules *rules, t_philosopher *philos);

// checks.c
int	check_args(int argc, char **argv, t_rules *rules);
void	print_status(t_philosopher *philo, char *status);

// init.c
int	init_simulation(t_rules *rules, t_philosopher **philos);
int	init_philos(t_rules *rules, t_philosopher **philos);
int	init_rules(t_rules *rules);

// simulation.c
int	create_simulation(t_philosopher *philos, pthread_t *monitor_thread);
int	check_death(t_philosopher *philo);
int	all_philos_ate_enough(t_philosopher *philos);
void	*routine(void *arg);
void	*monitor_routine(void *arg);
#endif
