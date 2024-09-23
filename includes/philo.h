/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:33:24 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/23 18:19:51 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

typedef struct l_philo
{
	int				n;
	int				eaten_meal;
	int				eating;
	int				dead;
	pthread_t		thread;
	long int		last_meal;
	struct l_info	*info;
	pthread_mutex_t	*fork_r;
	pthread_mutex_t	fork_l;
}		t_philo;

typedef struct l_info
{
	int				philo_eat;
	int				n_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	int				stop;
	long int		t_start;
	t_philo			*philosophers;
	pthread_mutex_t	print_lock;	
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	dead_lock;
	pthread_t		monitor_thread;
}		t_info;

// monitoring
void		*monitor(void *pointer);

// action
int			dead_loop(t_philo *philo);

// utils
int			ft_atoi(const char *str);
int			ft_isdigit(int character);
long long	timestamp(void);
void		*philo_life(void *phlshper);
void		ft_usleep(int ms);
void		t_print(t_philo *philo, char *str);
int			is_dead(t_philo *philo, int nb);

#endif
