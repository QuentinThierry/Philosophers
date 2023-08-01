/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:22:48 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/01 20:48:06 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	swap(size_t *v1, size_t *v2)
{
	size_t	tmp;

	tmp = *v1;
	*v1 = *v2;
	*v2 = tmp;
}

void	eat(t_philo *philo)
{
	size_t	id_l;
	size_t	id_r;
	long	cur_time;

	id_r = get_right_fork_id(philo);
	id_l = get_left_fork_id(philo);
	if (philo->id == philo->nb_philos)
		swap(&id_r, &id_l);
	pthread_mutex_lock(&philo->forks[id_r].mut);
	print_event(philo, "has taken a fork", get_timestamp(*philo->origin_time));
	pthread_mutex_lock(&philo->forks[id_l].mut);
	cur_time = get_timestamp(*philo->origin_time);
	print_event(philo, "has taken a fork", cur_time);
	pthread_mutex_lock(philo->mut_last_meal);
	philo->last_meal = cur_time;
	pthread_mutex_unlock(philo->mut_last_meal);
	philo->state = eating;
	print_event(philo, "is eating", cur_time);
	my_usleep(philo, philo->times[t_eat], cur_time);
	pthread_mutex_unlock(&philo->forks[id_l].mut);
	pthread_mutex_unlock(&philo->forks[id_r].mut);
	if (philo->nb_eat_max >= -2)
		(pthread_mutex_lock(philo->mut_eat_end),
			philo->nb_eat++,
			pthread_mutex_unlock(philo->mut_eat_end));
}
