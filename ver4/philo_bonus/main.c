int main(int argc, char **argv)
{
    t_table table;
    t_philosopher philo;
    
    // Инициализация структуры стола с помощью memset
    memset(&table, 0, sizeof(t_table));
    memset(&philo, 0, sizeof(t_philosopher));
    
    // Парсинг аргументов
    if (parse_arguments(argc, argv, &table))
    {
        printf("Ошибка: Неверные аргументы\n");
        return (1);
    }
    
    // Инициализация симуляции
    if (init_simulation(&table, &philo))
    {
        printf("Ошибка: Не удалось инициализировать симуляцию\n");
        return (1);
    }
    
    // Запуск симуляции
    if (start_simulation(&table, &philo))
    {
        printf("Ошибка: Не удалось запустить симуляцию\n");
        clean_simulation(&table);
        return (1);
    }
    
    // Если задано число приемов пищи, отслеживаем достижение цели
    if (table.num_times_to_eat > 0)
        monitor_meals(&table);
    
    // Очистка ресурсов и ожидание завершения процессов
    clean_simulation(&table);
    
    return (0);
}
