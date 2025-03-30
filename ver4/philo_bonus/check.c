// Парсинг аргументов командной строки
int parse_arguments(int argc, char **argv, t_table *table)
{
    if (!(argc == 5 || argc == 6))
        return (1);
    
    table->num_philosophers = atoi(argv[1]);
    table->time_to_die = atoi(argv[2]);
    table->time_to_eat = atoi(argv[3]);
    table->time_to_sleep = atoi(argv[4]);
    
    if (argc == 6)
        table->num_times_to_eat = atoi(argv[5]);
    else
        table->num_times_to_eat = -1;
    
    if (table->num_philosophers <= 0 || table->time_to_die <= 0 ||
        table->time_to_eat <= 0 || table->time_to_sleep <= 0 ||
        (argc == 6 && table->num_times_to_eat <= 0))
        return (1);
    
    return (0);
}
