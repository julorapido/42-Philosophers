/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:33:24 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/25 17:11:35 by jsaintho         ###   ########.fr       */
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
	long int		t_start;	
	t_philo			*philosophers;
	pthread_mutex_t	print_lock;	
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	dead_lock;	
	pthread_t		monitor;
}		t_info;

// monitoring
void		*monitor(void *pointer);

// action
int			dead_loop(t_philo *philo);

// init
void		init_mutexes(t_info	*d);
void		init_philos(t_info *d);
void		thread_create(t_info *d);
int			init_t_info(t_info *data, char **av);

// destroy
void		destroy_all(char *str, t_info *d);

// utils
long long	timestamp(void);
int			ft_atoi(const char *str);
int			ft_isdigit(int character);
void		*philo_life(void *phlshper);
void		ft_usleep(int ms);
void		t_print(t_philo *philo, char *str);
int			is_dead(t_philo *philo, int nb);

#endif
