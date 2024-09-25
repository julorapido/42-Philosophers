/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:06:51 by jsaintho          #+#    #+#             */
/*   Updated: 2024/09/25 17:13:34 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	signe;

	i = 0;
	signe = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == ' '))
		i++;
	if (str[i] == '-')
	{
		signe *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * signe);
}

long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

//long int	time;
//time = timestamp();
//while (timestamp() - time < ms)
//		usleep(ms / 10)
void	ft_usleep(int ms)
{
	usleep(ms * 1000);
}

void	t_print(t_philo *philo, char *str)
{
	long int	time;

	pthread_mutex_lock(&philo->info->print_lock);
	time = timestamp() - philo->info->t_start;
	if (!dead_loop(philo))
		printf("%ldms %d %s", time, philo->n, str);
	pthread_mutex_unlock(&philo->info->print_lock);
}
