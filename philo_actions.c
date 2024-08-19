/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:44:05 by jsaintho          #+#    #+#             */
/*   Updated: 2024/08/19 17:51:52 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_check_death(void *philo)
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

static void	philo_take_fork(t_philo *philo)
{
	pthread_mutex_lock(&(philo->fork_l));
	t_print(philo, " has taken a fork\n");
	if (philo->info->n_philo == 1)
	{
		ft_usleep(philo->info->t_die * 2);
		return ;
	}
	pthread_mutex_lock(philo->fork_r);
	t_print(philo, "has taken a fork \n");
}

static void	philo_eat(t_philo *philo)
{
	t_print(philo, " is eating\n");
	pthread_mutex_lock(&(philo->info->m_eat));
	philo->last_eat = timestamp();
	philo->m_count++;
	pthread_mutex_unlock(&(philo->info->m_eat));
	ft_usleep(philo->info->t_eat);
	pthread_mutex_unlock(philo->fork_r);
	pthread_mutex_unlock(&(philo->fork_l));
	t_print(philo, "is sleeping\n");
	ft_usleep(philo->info->t_sleep);
	t_print(philo, "is thinking \n");
}

void	*philo_life(void *phlshper)
{
	t_philo		*philo;
	pthread_t	t;

	philo = (t_philo *)phlshper;
	if(philo->n % 2 == 0)	
		ft_usleep(philo->info->t_eat / 10);
	while (!is_dead(philo, 0))
	{
		pthread_create(&t, NULL, philo_check_death, phlshper);
		philo_take_fork(philo);
		philo_eat(philo);
		pthread_detach(t);
		if (philo->m_count == philo->info->n_eat)
		{
			//pthread_mutex_lock();
			//pthread_mutex_unlock();
			return (NULL);
		}
	}
	return (phlshper);
}
