/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:12:41 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/25 13:07:18 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

// 0. Initalize t_info 
int	init_t_info(t_info *data, char **av)
{	
	data->n_philo = ft_atoi(av[1]);
	if (data->n_philo == 1)
	{
		printf("1ms 1 take his fork (L)\n");
		ft_usleep(ft_atoi(av[2]));
		printf("%ldms 1 died\n", (long)ft_atoi(av[2]));
		return (-1);
	}
	data->philosophers = (t_philo *) malloc(sizeof(t_philo) * data->n_philo);
	if (!data->philosophers)
		return (2);
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	data->t_sleep = ft_atoi(av[4]);
	data->n_eat = -1;
	if (av[5])
		data->n_eat = ft_atoi(av[5]);
	if (av[5] && data->n_eat == 0)
		return (1);
	return (0);
}

// 1. Initializing mutexes(fork && blockers)
void	init_mutexes(t_info	*d)
{	
	int	i;

	i = 0;
	while (i < d->n_philo)
	{
		pthread_mutex_init(&d->philosophers[i].fork_l, NULL);
		i++;
	}
	pthread_mutex_init(&d->print_lock, NULL);
	pthread_mutex_init(&d->dead_lock, NULL);
	pthread_mutex_init(&d->eat_lock, NULL);
}

// 2. Initializing the philosophers
void	init_philos(t_info *d)
{
	int	i;

	d->t_start = timestamp();
	i = 0;
	while (i < d->n_philo)
	{	
		d->philosophers[i].n = i + 1;
		d->philosophers[i].last_meal = timestamp();
		d->philosophers[i].dead = 0;
		d->philosophers[i].fork_r = NULL;
		d->philosophers[i].info = d;
		d->philosophers[i].eaten_meal = 0;
		d->philosophers[i].eating = 0;
		if (i == 0)
			d->philosophers[i].fork_r = &d->philosophers[d->n_philo - 1].fork_l;
		else
			d->philosophers[i].fork_r = &d->philosophers[i - 1].fork_l;
		i++;
	}	
}

// 3. Creates all the threads
void	thread_create(t_info *d)
{
	int	i;

	if (pthread_create(&d->monitor, NULL, &monitor, d) != 0)
		destroy_all("Thread creation error", d);
	i = 0;
	while (i < d->n_philo)
	{
		if (pthread_create(&d->philosophers[i].thread,
				NULL, &philo_life, &d->philosophers[i]) != 0)
			destroy_all("Thread creation error", d);
		i++;
	}
	i = 0;
	if (pthread_join(d->monitor, NULL) != 0)
		destroy_all("Thread join error", d);
	while (i < d->n_philo)
	{
		if (pthread_join(d->philosophers[i].thread, NULL) != 0)
			destroy_all("Thread join error", d);
		i++;
	}
}
