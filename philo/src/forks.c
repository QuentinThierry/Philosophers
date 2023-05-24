/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:23:37 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/24 18:24:00 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	try_take_l_fork(t_philo *philo,
							t_timeval *act_time, double *timestamps)
{
	size_t	id;

	if (!philo->has_l_fork)
	{
		id = get_left_fork_id(philo);
		pthread_mutex_lock(&philo->forks[id].mut);
		if (!philo->forks[id].is_taken)
		{
			philo->forks[id].is_taken = true;
			philo->has_l_fork = true;
			print_event(philo, act_time, timestamps, "has taken a fork");
		}
		pthread_mutex_unlock(&philo->forks[id].mut);
	}
	return (true);
}

static bool	try_take_r_fork(t_philo *philo,
							t_timeval *act_time, double *timestamps)
{
	size_t	id;

	if (!philo->has_r_fork)
	{
		id = get_right_fork_id(philo);
		pthread_mutex_lock(&philo->forks[id].mut);
		if (!philo->forks[id].is_taken)
		{
			philo->forks[id].is_taken = true;
			philo->has_r_fork = true;
			print_event(philo, act_time, timestamps, "has taken a fork");
		}
		pthread_mutex_unlock(&philo->forks[id].mut);
	}
	return (true);
}

bool	try_take_forks(t_philo *philo, t_timeval *act_time, double *timestamps)
{
	if (philo->nb_philos == 1)
		return (true);
	try_take_l_fork(philo, act_time, timestamps);
	try_take_r_fork(philo, act_time, timestamps);
	return (true);
}

bool	release_forks(t_philo *philo)
{
	size_t	id;

	id = get_left_fork_id(philo);
	pthread_mutex_lock(&philo->forks[id].mut);
	philo->forks[id].is_taken = false;
	philo->has_l_fork = false;
	pthread_mutex_unlock(&philo->forks[id].mut);
	id = get_right_fork_id(philo);
	pthread_mutex_lock(&philo->forks[id].mut);
	philo->forks[id].is_taken = false;
	philo->has_r_fork = false;
	pthread_mutex_unlock(&philo->forks[id].mut);
	return (true);
}
