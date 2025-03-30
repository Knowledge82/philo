#include "philo.h"

// Получить текущее время в миллисекундах
long long get_time_ms(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// Точная задержка в миллисекундах
void precise_sleep(long long time_ms)
{
    long long start;

    start = get_time_ms();
    while (get_time_ms() - start < time_ms)
        usleep(100);
}

// Вывод сообщения о состоянии философа
void print_status(t_philosopher *philo, char *status)
{
    long long current_time;

    sem_wait(philo->table->print_sem);
    current_time = get_time_ms() - philo->table->start_time;
    printf("%lld %d %s\n", current_time, philo->id, status);
    sem_post(philo->table->print_sem);
}
