/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarinov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:57:20 by mmarinov          #+#    #+#             */
/*   Updated: 2025/02/18 11:59:25 by mmarinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

# ifndef COLOR_H
#  define COLOR_H

#  define RED   "\033[1;31m"
#  define GR    "\033[1;32m"
#  define LGR   "\033[1;92m"
#  define LGREY "\033[38;5;250m"
#  define GREY  "\033[1;90m"
#  define YEL   "\033[1;33m"
#  define OR    "\033[38;5;214m"
#  define BL    "\033[1;34m"
#  define MAG   "\033[1;35m"
#  define CYAN  "\033[1;36m"
#  define RES   "\033[0m"
# endif

typedef struct s_fork
{
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_filo
{
	int				id;
	pthread_t		thread;
	long long		last_meal;
	t_fork			*rf;
	t_fork			*lf;
	int				meals_done;
	struct s_dta	*dta;
}					t_filo;

typedef struct s_dta
{
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_lock;
	t_filo			*filos;
	t_fork			*forks;
	bool			death;
	bool			full;
	long long		tto_die;
	long long		tto_eat;
	long long		tto_sleep;
	long long		start_time;
	int				n_filos;
	int				n_meals;
}					t_dta;

int			ft_atoi(const char *s);
long long	time_now(void);
int			ft_usleep(long long time);
void		error_exit(const char *error);
void		ft_prints(t_dta *dta, int id, const char *acts);
void		take_forks(t_filo *filo);
void		put_forks(t_filo *filo);
void		eat(t_filo *filo);
void		think(t_filo *filo);
void		sleep_filo(t_filo *filo);
void		*lifecycle(void *arg);
void		check_death(t_dta *dta);
void		*monitor(void *arg);
void		init_dta(t_dta *dta);
int			init_filos(t_dta *dta);
void		free_resources(t_dta *dta);
int			check_args(int ac, char **av);
int			check_int_count(int ac);
int			check_num_filo(char *arg);
int			check_params(char *arg);

#endif
