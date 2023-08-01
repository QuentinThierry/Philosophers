/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:22:17 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/01 19:12:41 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	get_is_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_end);
	if (*philo->is_end)
	{
		pthread_mutex_unlock(philo->mut_end);
		return (true);
	}
	pthread_mutex_unlock(philo->mut_end);
	return (false);
}

size_t	get_left_fork_id(t_philo *philo)
{
	if (philo->id == philo->nb_philos)
		return (0);
	return (philo->id);
}

size_t	get_right_fork_id(t_philo *philo)
{
	return (philo->id - 1);
}
