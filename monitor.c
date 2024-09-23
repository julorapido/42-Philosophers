/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:29:22 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/23 18:18:37 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


// CHECK [ONE] PHILO DEATH
static int	philosopher_dead(t_philo *philo, int time_to_die)
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
static int	check_if_dead(t_philo *philos, int n, int d)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (philosopher_dead(&philos[i], d))
		{
			t_print(&philos[i],"died");
			pthread_mutex_lock(&philos[i].info->dead_lock);
			philos[i].dead = 1;
			pthread_mutex_unlock(&philos[i].info->dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

// Checks if all the philos ate the num_of_meals
/*static int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].num_times_to_eat == -1)
		return (0);
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[i].meal_lock);
		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
			finished_eating++;
		pthread_mutex_unlock(philos[i].meal_lock);
		i++;
	}
	if (finished_eating == philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos->dead = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}*/


void	*monitor(void *p)
{
	t_info	*td;
	t_philo	*philo;

	td = (t_info *)p;
	philo = (td)->philosophers;
	while (1)
		if (check_if_dead(philo, td->n_philo, td->t_die) == 1) // || check_if_all_ate(philos) == 1)
			break ;
	return (p);
}
