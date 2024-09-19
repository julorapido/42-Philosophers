/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:44:05 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/19 13:00:18 by jsaintho         ###   ########.fr       */
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

void	*philo_check_death(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	ft_usleep(p->info->t_die + 1);
	pthread_mutex_lock(&p->info->m_eat);
	pthread_mutex_lock(&p->info->m_stop);
	if (!is_dead(p, 0) && timestamp() - p->last_eat >= (long)(p->info->t_die))
	{
		pthread_mutex_unlock(&p->info->m_eat);
		pthread_mutex_unlock(&p->info->m_stop);
		t_print(p, " died\n");
		is_dead(p, 1);
	}
	pthread_mutex_unlock(&p->info->m_eat);
	pthread_mutex_unlock(&p->info->m_stop);
	return (NULL);
}

void	philo_take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo->fork_l));
	t_print(philo, " has taken a fork (L) \n");
	if (philo->info->n_philo == 1)
	{
		ft_usleep(philo->info->t_die * 2);
		return ;
	}
	pthread_mutex_lock(philo->fork_r);
	t_print(philo, " has taken a fork (R) \n");
}

void	philo_eat_sleep_think(t_philo *philo)
{
	t_print(philo, " is eating\n");
	pthread_mutex_lock(&(philo->info->m_eat));
	philo->last_eat = timestamp();
	philo->m_count++;
	pthread_mutex_unlock(&(philo->info->m_eat));
	ft_usleep(philo->info->t_eat);
	pthread_mutex_unlock(philo->fork_r);
	pthread_mutex_unlock(&(philo->fork_l));
	t_print(philo, " is sleeping\n");
	ft_usleep(philo->info->t_sleep);
	t_print(philo, " is thinking\n");
}

void	*philo_life(void *phlshper)
{	
	t_philo		*philo;
	pthread_t	t;

	philo = (t_philo *)phlshper;
	if (philo->n % 2 == 0)
		ft_usleep(philo->info->t_eat / 10);
	while (!is_dead(philo, 0))
	{
		pthread_create(&t, NULL, philo_check_death, phlshper);
		philo_take_fork(philo);
		philo_eat_sleep_think(philo);
		pthread_detach(t);
		if (philo->m_count == philo->info->n_eat)
		{
			pthread_mutex_lock(&philo->info->m_stop);
			if (++philo->info->philo_eat == philo->info->n_philo)
			{
				pthread_mutex_unlock(&philo->info->m_stop);
				is_dead(philo, 2);
			}
			pthread_mutex_unlock(&philo->info->m_stop);
			return (NULL);
		}
	}
	return (NULL);
}
