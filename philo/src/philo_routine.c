/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:34:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/29 15:52:02 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static inline bool	begin_sleep(t_philo *philo)
{
	pthread_mutex_lock(philo->mut_eat_end);
	if (philo->eat_to_end > -1)
		philo->eat_to_end--;
	pthread_mutex_unlock(philo->mut_eat_end);
	print_event(philo, "is sleeping");
	philo->begin_sleep = get_timestamp(*philo->start_time);
	philo->state = get_next_state(philo->state);
	release_forks(philo);
	return (true);
}

static inline bool	begin_think(t_philo *philo)
{
	print_event(philo, "is thinking");
	philo->state = get_next_state(philo->state);
	return (true);
}

static inline bool	philo_routine2(t_philo *philo)
{
	if (philo->state == thinking)
	{
		try_to_eat(philo);
	}
	else if (philo->state == eating
		&& get_time_diff(*philo->start_time, philo->begin_eat)
		>= philo->times[t_eat])
	{
		if (!begin_sleep(philo))
			return (false);
	}
	else if (philo->state == sleeping
		&& get_time_diff(*philo->start_time, philo->begin_sleep)
		>= philo->times[t_sleep])
	{
		if (!begin_think(philo))
			return (false);
	}
	return (true);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep(500);
	while (true)
	{
		if (get_is_dead(philo))
			return (NULL);
		if (!philo_routine2(philo))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
