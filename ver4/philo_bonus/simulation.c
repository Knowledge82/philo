#include "philo.h"

// Мониторинг состояния философа (проверка на смерть)
void *monitor_routine(void *arg)
{
    t_philosopher *philo;
    
    philo = (t_philosopher *)arg;
    while (1)
    {
        sem_wait(philo->table->meal_sem);
        if (get_time_ms() - philo->last_meal_time > philo->table->time_to_die)
        {
            sem_wait(philo->table->print_sem);
            printf("%lld %d died\n", get_time_ms() - philo->table->start_time, philo->id);
            exit(1);  // Завершение процесса философа при смерти
        }
        sem_post(philo->table->meal_sem);
        usleep(1000);
    }
    return (NULL);
}

// Процесс философа
void philosopher_process(t_philosopher *philo)
{
    // Создание потока для мониторинга смерти философа
    if (pthread_create(&philo->monitor_thread, NULL, monitor_routine, philo) != 0)
        exit(1);
    if (pthread_detach(philo->monitor_thread) != 0)
        exit(1);
    
    // Небольшая задержка для нечетных философов, чтобы избежать дедлоков
    if (philo->id % 2 == 0)
        precise_sleep(1);
    
    while (1)
    {
        // Размышление
        print_status(philo, "is thinking");
        
        // Взятие вилок (семафоры)
        sem_wait(philo->table->forks_sem);
        print_status(philo, "has taken a fork");
        sem_wait(philo->table->forks_sem);
        print_status(philo, "has taken a fork");
        
        // Прием пищи
        print_status(philo, "is eating");
        sem_wait(philo->table->meal_sem);
        philo->last_meal_time = get_time_ms();
        philo->meals_eaten++;
        sem_post(philo->table->meal_sem);
        
        precise_sleep(philo->table->time_to_eat);
        
        // Если философ съел достаточно, сигнализируем об этом
        if (philo->table->num_times_to_eat > 0 && 
            philo->meals_eaten >= philo->table->num_times_to_eat)
            sem_post(philo->table->finished_sem);
        
        // Освобождение вилок
        sem_post(philo->table->forks_sem);
        sem_post(philo->table->forks_sem);
        
        // Сон
        print_status(philo, "is sleeping");
        precise_sleep(philo->table->time_to_sleep);
    }
    exit(0);
}
// Запуск симуляции (создание процессов философов)
int start_simulation(t_table *table, t_philosopher *philo)
{
    int i;
    pid_t pid;

    i = 0;
    while (i < table->num_philosophers)
    {
        pid = fork();

        if (pid == -1)
        {
            // Ошибка создания процесса
            i = 0;
            while (i < table->num_philosophers && table->pids[i] > 0)
            {
                kill(table->pids[i], SIGKILL);
                i++;
            }
            return (1);
        }
        else if (pid == 0)
        {
            // Дочерний процесс (философ)
            philo->id = i + 1;
            philosopher_process(philo);
            exit(0); // Не должны дойти до этой точки
        }
        else
        {
            // Родительский процесс (сохраняем PID)
            table->pids[i] = pid;
        }
        i++;
    }
    return (0);
}

// Мониторинг завершения всех философов по количеству приемов пищи
void monitor_meals(t_table *table)
{
    int i;

    i = 0;
    while (i < table->num_philosophers)
    {
        sem_wait(table->finished_sem);
        i++;
    }

    // Если все философы поели достаточно раз, завершаем всех
    i = 0;
    while (i < table->num_philosophers)
    {
        kill(table->pids[i], SIGKILL);
        i++;
    }
}

// Ожидание завершения процессов и освобождение ресурсов
void clean_simulation(t_table *table)
{
    int i;
    int status;

    // Ожидание всех процессов если не завершены по числу приемов пищи
    if (table->num_times_to_eat < 0 || table->num_philosophers <= 0)
    {
        i = 0;
        while (i < table->num_philosophers)
        {
            waitpid(table->pids[i], &status, 0);
            if (WEXITSTATUS(status) == 1)
            {
                // Если один философ умер, завершаем все процессы
                int j = 0;
                while (j < table->num_philosophers)
                {
                    if (j != i)
                        kill(table->pids[j], SIGKILL);
                    j++;
                }
                break;
            }
            i++;
        }
    }

    // Закрытие и удаление семафоров
    sem_close(table->forks_sem);
    sem_close(table->print_sem);
    sem_close(table->meal_sem);
    sem_close(table->finished_sem);

    sem_unlink("/forks");
    sem_unlink("/print");
    sem_unlink("/meal");
    sem_unlink("/finished");

    // Освобождение памяти
    free(table->pids);
}
