/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:22:48 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/25 20:28:44 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	try_to_eat(t_philo *philo, t_timeval *act_time, double *timestamps)
{
	try_take_forks(philo, act_time, timestamps);
	if (philo->has_l_fork && philo->has_r_fork)
	{
		philo->state = eating;
		print_event(philo, act_time, timestamps, "is eating");
		philo->begin_eat = get_timestamp(philo->start_time);
	}
}

bool	has_all_eaten(t_philo *philos)
{
	size_t	i;

	if (philos->eat_to_end == -2) //to_change
		return (false);
	i = 0;
	pthread_mutex_lock(philos->mut_eat_end);
	while (i < philos->nb_philos)
	{
		if (philos[i].eat_to_end > 0)
		{
			pthread_mutex_unlock(philos->mut_eat_end);
			return (false);
		}
		i++;
	}
	pthread_mutex_unlock(philos->mut_eat_end);
	return (true);
}
