/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:29:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/24 15:34:07 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// CHECK [ONE] PHILO DEATH
int	philosopher_dead(t_philo *philo, int time_to_die)
{
	pthread_mutex_lock(&philo->info->eat_lock);
	if (timestamp() - philo->last_meal >= time_to_die && philo->eating == 0)
	{
		return (pthread_mutex_unlock(&philo->info->eat_lock), 1);
	}
	pthread_mutex_unlock(&philo->info->eat_lock);
	return (0);
}

// CHECK [ANY] PHILO DEATH
int	check_if_dead(t_philo *philos, int n, int d)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (philosopher_dead(&philos[i], d))
		{
			t_print(&philos[i], "died\n");
			pthread_mutex_lock(&philos[0].info->dead_lock);
			philos[0].dead = 1;
			pthread_mutex_unlock(&philos[0].info->dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

// Checks if all the philos ate the num_of_meals
int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].info->n_eat < 0)
		return (0);
	while (i < philos[0].info->n_philo)
	{
		pthread_mutex_lock(&philos[i].info->eat_lock);
		if (philos[i].eaten_meal >= philos[i].info->n_eat)
			finished_eating++;
		pthread_mutex_unlock(&philos[i].info->eat_lock);
		i++;
	}
	if (finished_eating == philos[0].info->n_philo)
	{
		pthread_mutex_lock(&philos[0].info->dead_lock);
		philos[0].dead = 1;
		pthread_mutex_unlock(&philos[0].info->dead_lock);
		return (1);
	}
	return (0);
}

void	*monitor(void *p)
{
	t_info	*td;

	td = (t_info *)p;
	while (1)
	{
		if (check_if_dead(td->philosophers, td->n_philo, td->t_die) == 1
			|| (check_if_all_ate(td->philosophers) == 1))
			break ;
	}
	return (p);
}
