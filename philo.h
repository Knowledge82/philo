#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define RESET   "\033[0m"

typedef struct	s_rules
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat; // сколько должен поесть философ(-1, если не указано)
	int	simulation_end;
	int	philos_full; // наевшиеся
	long long	start_time; // время старта симуляции
	pthread_mutex_t	write_mutex; // мьютекс для вывода в консоль
	pthread_mutex_t	*forks; // массив мьютексов-вилок
	pthread_mutex_t	death_mutex; // мьютекс для защиты флага
	struct t_philosopher	*philos; // массив философов
} t_rules;

typedef struct	s_philosopher
{
	int	id; // номер философа
	int	meals_eaten; //количество съеденых блюд
	long long	last_meal_time; //время последнего приёма пищи
	pthread_t	thread; // поток философа
	pthread_mutex_t	*left_fork; //вилка слева
	pthread_mutex_t	*right_fork; //вилка справа
	t_rules	*rules;
} t_philosopher;

//routine.c
void	*life_routine(void *arg);

//check_and_init.c
int	is_valid_arg(char *str);
int	check_args(int argc, char **argv);
int	init_rules(t_rules *rules, int argc, char **argv);
int	init_mutexes(t_rules *rules, pthread_mutex_t *forks);
int	init_philosophers(t_philosopher **philos, pthread_mutex_t *forks, t_rules *rules);
int	create_philosophers_threads(t_philosopher *philos);
int	create_monitor_thread(pthread_t *monitor_thread, t_philosopher *philos);


//utils.c
int	ft_strlen(char *str);
void	ft_print_msg(t_philosopher *philo, const char *msg);
void	error_message(char *text);
long long	get_time(void);
void	*monitor_routine(void *arg);
int	is_valid_arg(char *str);
int check_simulation_end(t_rules *rules);
void	ft_wait(int duration, t_rules *rules);
