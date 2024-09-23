/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:44:05 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/23 18:12:43 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ================== PIHLOSOPHER LIFE ===========================
// (Make 2-nth%2's philosopher sleep to delay eatin) 
// == 1. New thread to check philo death (lock EAT and STOP)
// = 	- verify that philo's timestap() - last_eat > time_to_die
// = 	- if(philo is dead) info->stop == 1
// == 2. Philosopher takes his forks (wait lock FORK_L)
// =	- (lock his r_fork[] and his l_fork[fork of his neighbor])
// == 3. Philosopher eats-sleeps-thinks (waits lock EAT)
// =	- m_count++ and last_eat = timestamp()
// = 	- thread sleep(time_eat)
// =	- unlock (fork_l and fork_r)
// =	- Sleeps! 
// =	- Thinks!
// ===============================================================
// ofc 
// use another thread specific for checking philo deaths (pthread_detach)


// THINK
void	think(t_philo *philo)
{
	t_print(philo, "is thinking");
}

// SLEEP
void	sleeep(t_philo *philo)
{
	t_print(philo, "is sleeping");
	ft_usleep(philo->info->t_sleep);
}

// EAT
void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_l);
	t_print(philo, "has taken a fork (L)");
	if (philo->info->n_philo == 1)
	{
		ft_usleep(philo->info->t_die);
		pthread_mutex_unlock(&philo->fork_l);
		return ;
	}
	pthread_mutex_lock(philo->fork_r);
	t_print(philo, "has taken a fork (R)");
	philo->eating = 1;
	t_print(philo, "is eating");
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
	if (philo->dead == 1)
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
	if (philo->n % 2 == 0)
		ft_usleep(philo->info->t_eat / 10);	
	while (!dead_loop(philo))
	{
		eat(philo);
		sleeep(philo);
		think(philo);
	}
	return (NULL);
}
