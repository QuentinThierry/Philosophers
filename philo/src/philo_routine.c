/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:34:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/06 18:57:31 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static inline bool	begin_sleep(t_philo *philo)
{
	long	cur_time;

	cur_time = get_timestamp(*philo->origin_time);
	philo->state = sleeping;
	print_event(philo, "is sleeping", cur_time);
	my_usleep(philo, philo->times[t_sleep], cur_time);
	return (true);
}

static inline bool	begin_think(t_philo *philo)
{
	int	think_time;
	long	cur_time;

	cur_time = get_timestamp(*philo->origin_time);
	print_event(philo, "is thinking", cur_time);
	philo->state = thinking;
	think_time = 0;
	if (philo->nb_philos % 2 == 1 && philo->times[t_sleep] < philo->times[t_eat])
		think_time = philo->times[t_eat] - philo->times[t_sleep];
	my_usleep(philo, think_time, cur_time);
	return (true);
}

static inline bool	philo_routine2(t_philo *philo)
{
	if (philo->state == thinking)
	{
		eat(philo);
	}
	else if (philo->state == eating)
	{
		begin_sleep(philo);
	}
	else if (philo->state == sleeping)
	{
		begin_think(philo);
	}
	return (true);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;

	philo = arg;
	if (philo->nb_philos % 2 == 1)
	{
		if (philo->id % 2 == 1)
			usleep(1000);
	}
	if (philo->id % 2 == 0)
		philo->state = eating;
	while (true)
	{
		if (get_is_dead(philo))
			return (NULL);
		if (!philo_routine2(philo))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
