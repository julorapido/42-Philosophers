/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:35:23 by jsaintho          #+#    #+#             */
/*   Updated: 2024/08/19 15:39:54 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ====================== PHILOSOPHERS =======================
// 1 philosopher = 1 fork = 1 thread
// 1 philosopher needs 2-forks to eat
// each fork is a mutex
// then 1 philosopher got 2-forks 
//	[
//		LEFT : his own fork
//		RIGTH : left fork of nxt philosopher
//	]

static int	check_only_num(char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (argv[i++])
	{
		j = -1;
		while (argv[i][j++])
		{
			if(!ft_isdigit(argv[i][j]))
				return (1);
		}
	}
	return (0);
}

static int	philosophers_init(t_info *d)
{
	int	i;

	d->t_start = timestamp();
	i = -1;
	while(i++ < d->n_philo)
	{
		d->philosophers[i].n = i + 1;
		d->philosophers[i].last_eat = 0;
		d->philosophers[i].fork_r = NULL;
		d->philosophers[i].info = d;
		d->philosophers[i].m_count = 0;
		pthread_mutex_init(&(d->philosophers[i].fork_l), NULL);
		if(i == d->n_philo - 1)
			d->philosophers[i].fork_r = &d->philosophers[0].fork_l;
		else
			d->philosophers[i].fork_r = &d->philosophers[i + 1].fork_l;
		if (pthread_create(&d->philosophers[i].thread, NULL, &philo_life, &(d->philosophers[i])) != 0)
			return (-1);
	}
	i = -1;
	while (i++ < d->n_philo)
		if (pthread_join(d->philosophers[i].thread, NULL) != 0)
			return (-1);
	return (0);
}

static int	t_info_init(t_info *data, char **av)
{
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->m_eat, NULL);
	pthread_mutex_init(&data->m_stop, NULL);
	pthread_mutex_init(&data->dead, NULL);
	data->stop = 0;
	data->philosophers = (t_philo *) malloc(sizeof(t_philo) * data->n_philo);
	if(!data->philosophers)
		return (2);
	if (check_only_num(av))
	{
		printf("Invalid Arguments \n");
		return (1);
	}
	data->n_philo = ft_atoi(av[1]);
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	data->t_sleep = ft_atoi(av[4]);
	if (av[5])
		data->n_eat = ft_atoi(av[5]);
	if (av[5] && data->n_eat == 0)
		return (1);
	return (0);
}

int main(int ac, char **argv)
{
	t_info	data;

	if (ac != 5 && ac != 6)
			return (EXIT_FAILURE);
	if (t_info_init(&data, argv) == 1)
	{
		free(data.philosophers);
		return (EXIT_FAILURE);
	}
	philosophers_init(&data);
	return (EXIT_SUCCESS);
}
