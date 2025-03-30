#ifndef PHILO_H
# define PHILO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>

typedef struct s_rules
{
    int             num_philosophers;
    long long       time_to_die;
    long long       time_to_eat;
    long long       time_to_sleep;
    int             num_times_to_eat;
    long long       start_time;
    sem_t           *forks_sem;
    sem_t           *print_sem;
    sem_t           *meal_sem;
    sem_t           *finished_sem;
    pid_t           *pids;
} t_rules;

typedef struct s_philosopher
{
    int             id;
    int             meals_eaten;
    long long       last_meal_time;
    pthread_t       monitor_thread;
    t_table         *table;
} t_philosopher;

//prots
