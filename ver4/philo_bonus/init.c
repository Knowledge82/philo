// Инициализация семафоров
int init_semaphores(t_table *table)
{
    // Удаляем семафоры, если они уже существуют
    sem_unlink("/forks");
    sem_unlink("/print");
    sem_unlink("/meal");
    sem_unlink("/finished");
    
    // Создаем новые семафоры
    table->forks_sem = sem_open("/forks", O_CREAT, 0644, table->num_philosophers);
    table->print_sem = sem_open("/print", O_CREAT, 0644, 1);
    table->meal_sem = sem_open("/meal", O_CREAT, 0644, 1);
    table->finished_sem = sem_open("/finished", O_CREAT, 0644, 0);
    
    if (table->forks_sem == SEM_FAILED || table->print_sem == SEM_FAILED ||
        table->meal_sem == SEM_FAILED || table->finished_sem == SEM_FAILED)
        return (1);
    
    return (0);
}

// Инициализация таблицы и философов
int init_simulation(t_table *table, t_philosopher *philo)
{
    // Инициализация семафоров
    if (init_semaphores(table))
        return (1);
    
    // Выделение памяти для PID процессов
    table->pids = malloc(sizeof(pid_t) * table->num_philosophers);
    if (!table->pids)
        return (1);
    
    // Инициализация времени начала симуляции
    table->start_time = get_time_ms();
    
    // Инициализация структуры философа
    philo->id = 0;
    philo->meals_eaten = 0;
    philo->last_meal_time = get_time_ms();
    philo->table = table;
    
    return (0);
}

