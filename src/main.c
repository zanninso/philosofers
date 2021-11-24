#include "philo.h"

_Bool get_args(int ac, char **av, t_env *env)
{
    if (ac < 5)
        return (false);
    env->philos_number = ft_atoi(av[1]);
    env->die_time = ft_atoi(av[2]);
    env->eating_time = ft_atoi(av[3]) * 1000;
    env->sleeping_time = ft_atoi(av[4]) * 1000;
    if (ac > 5)
        env->should_eat_counter = ft_atoi(av[5]);
    return true;
}

_Bool init(t_env *env)
{
    size_t i;

    env->philos = malloc(env->philos_number * sizeof(t_philo));
    env->forks = malloc(env->philos_number * sizeof(t_fork));
    env->thread_ids = malloc(env->philos_number * sizeof(pthread_t));
    i = 0; 
    if (env->philos)
        while (i < env->philos_number)
            init_philo(i++, env);
    i = 0; 
    if (env->forks)
        while (i < env->philos_number)
        {
            env->forks[i].status = FORK_FREE;
            pthread_mutex_init(&env->forks[i].lock, NULL);
            i++;
        }
    pthread_mutex_init(&env->printer_lock, NULL);
    pthread_mutex_init(&env->born_lock, NULL);
    return (env->philos && env->forks && env->thread_ids);
}

void destroy(t_env *env)
{
    size_t i;

    i = 0; 
    if (env->thread_ids)
        while (i < env->philos_number)
            pthread_mutex_destroy(&env->forks[i++].lock);
    pthread_mutex_destroy(&env->born_lock);
    pthread_mutex_destroy(&env->printer_lock);
    free(env->philos);
    free(env->forks);
    free(env->thread_ids);
}

void create_threads(t_env *env)
{
    size_t i;

    i = 0;
    while (i < env->philos_number)
    {
        pthread_create(&env->thread_ids[i], NULL, born_philo, env);
        i++;
    }

    while (true)
    {
        i = 0;
        usleep(1000);
        size_t current_time = get_timestamp();
        while (i < env->philos_number)
        {
            if (current_time > env->philos[i].expected_death_time)
            {
                printf(DIED_MSG, get_timestamp(), i);
                return;
            }
            i++;
        }
    }
}

int main(int ac, char **av)
{
    t_env env;

    memset(&env, 0, sizeof(t_env));
    if (get_args(ac, av, &env) && init(&env))
    {
        create_threads(&env);
    }
    destroy(&env);
}