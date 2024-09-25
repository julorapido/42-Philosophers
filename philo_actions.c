/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:44:05 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/25 17:23:50 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// THINK
void	think(t_philo *philo)
{	
	t_print(philo, "is thinking\n");
}

// SLEEP
void	sleeep(t_philo *philo)
{
	t_print(philo, "is sleeping\n");
	ft_usleep(philo->info->t_sleep);
}

// EAT
void	eat(t_philo *philo)
{
	if (philo->n % 2 == 1)
	{	
		usleep(philo->info->t_die);
		pthread_mutex_lock(&philo->fork_l);
		t_print(philo, "has taken a fork (L)\n");
		pthread_mutex_lock(philo->fork_r);
		t_print(philo, "has taken a fork (R)\n");
	}
	else
	{
		pthread_mutex_lock(philo->fork_r);
		t_print(philo, "has taken a fork (R)\n");
		pthread_mutex_lock(&philo->fork_l);
		t_print(philo, "has taken a fork (L)\n");
	}	
	philo->eating = 1;
	t_print(philo, "is eating\n");
	pthread_mutex_lock(&philo->info->eat_lock);
	philo->last_meal = timestamp();
	philo->eaten_meal++;
	pthread_mutex_unlock(&philo->info->eat_lock);
	ft_usleep(philo->info->t_eat);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->fork_l);
	pthread_mutex_unlock(philo->fork_r);
}

// Checks if the value of dead_flag changed
int	dead_loop(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->dead_lock);
	if ((philo->info->philosophers[0]).dead == 1)
	{
		return (pthread_mutex_unlock(&philo->info->dead_lock), 1);
	}
	pthread_mutex_unlock(&philo->info->dead_lock);
	return (0);
}

void	*philo_life(void *phlshper)
{	
	t_philo		*philo;

	philo = (t_philo *)phlshper;
	pthread_mutex_lock(&philo->info->eat_lock);
	pthread_mutex_unlock(&philo->info->eat_lock);
	while (!dead_loop(philo))
	{
		eat(philo);
		sleeep(philo);
		think(philo);
	}
	return (NULL);
}
