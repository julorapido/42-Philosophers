/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:35:23 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/25 12:25:14 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ====================== PHILOSOPHERS =======================
// 1 philosopher = 1 fork = 1 thread
// 1 philosopher needs 2-forks to eat
// 1 fork = 1 mutex
// then 1 philosopher got 2-forks 
//	[
//		LEFT : his own fork
//		RIGTH : left fork of nxt philosopher
//	]
//			[eat mutex
//			[print mutex
//	TABLE = [stop mutex
//			[
//			[

void	destroy_all(char *str, t_info *d)
{
	int	i;

	i = 0;
	if (str[0] != '\0')
	{
		printf("%s \n", str);
	}
	pthread_mutex_destroy(&d->print_lock);
	pthread_mutex_destroy(&d->eat_lock);
	pthread_mutex_destroy(&d->dead_lock);
	while (i < d->n_philo)
	{
		pthread_mutex_destroy(&d->philosophers[i].fork_l);
		i++;
	}
	free(d->philosophers);
}

int	check_only_num(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **argv)
{
	t_info	data;
	int		e;

	if ((ac != 5 && ac != 6) || check_only_num(argv))
	{
		printf("This program requires 4-5 arguments[INTERGERS ONLY].\n");
		printf("Arguments -philos_nbr -time_die -time_eat -time_sleep\n");
		return (EXIT_FAILURE);
	}
	e = init_t_info(&data, argv);
	if (e == -1)
		return (EXIT_SUCCESS);
	init_mutexes(&data);
	init_philos(&data);
	thread_create(&data);
	destroy_all("", &data);
	return (EXIT_SUCCESS);
}
