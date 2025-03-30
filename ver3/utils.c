#include "philo.h"

int	ft_atoi(const char *str)
{
	long long	num;

	num = 0;
	while(*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
	{
		printf("Error: only positive values allowed\n");
		return (-1);
	}
	else if (*str == '+')
		str++;
	while(*str >= '0' && *str <= '9')
	{
		num = num * 10 + (*str - '0');
		if (num > INT_MAX)
		{
			printf("Error: only values less then INT_MAX allowed\n");
			return (-1);
		}
		str++;
	}
	if (*str != '\0')
		return (-1);
	return ((int)num);
}

long long	get_time_ms(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	ft_sleep(long long duration, t_rules *rules)
{
	long long	start;
	long long	current;

	start = get_time_ms();
	while(!rules->death || !rules->all_full)
	{
	/*	pthread_mutex_lock(&rules->death_mutex);
        if (rules->death || rules->all_full)
        {
            pthread_mutex_unlock(&rules->death_mutex);
            break;
        }
        pthread_mutex_unlock(&rules->death_mutex);
	*/
		current = get_time_ms();
		if(current - start >= duration)
			break;
		if (duration > 1000)
			usleep(500);
		else
			usleep(50);
	}
}

void	print_msg(t_rules *rules, int id, char *msg)
{
	int	end;

	pthread_mutex_lock(&rules->death_mutex);
	
	if(rules->death || rules->all_full)
		end = 1;
	else
		end = 0;
	pthread_mutex_unlock(&rules->death_mutex);
	if (end)
		return ;
	pthread_mutex_lock(&rules->write_mutex);
	printf("%lli ", get_time_ms() - rules->start_simulation);
	printf("%i ", id + 1);
	printf("%s\n", msg);
	pthread_mutex_unlock(&rules->write_mutex);	
}
