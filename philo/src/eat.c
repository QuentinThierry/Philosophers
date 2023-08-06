/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:22:48 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/06 18:23:45 by qthierry         ###   ########.fr       */
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

static void	eat2(t_philo *philo, long cur_time, size_t id_l, size_t id_r)
{
	pthread_mutex_lock(philo->mut_last_meal);
	philo->last_meal = cur_time;
	pthread_mutex_unlock(philo->mut_last_meal);
	philo->state = eating;
	print_event(philo, "is eating", cur_time);
	my_usleep(philo, philo->times[t_eat], cur_time);
	pthread_mutex_unlock(&philo->forks[id_r].mut);
	pthread_mutex_unlock(&philo->forks[id_l].mut);
	if (philo->nb_eat_max >= -2)
		(pthread_mutex_lock(philo->mut_eat_end),
			philo->nb_eat++,
			pthread_mutex_unlock(philo->mut_eat_end));
}

void	eat(t_philo *philo)
{
	size_t	id_l;
	size_t	id_r;
	long	cur_time;

	id_r = get_right_fork_id(philo);
	id_l = get_left_fork_id(philo);
	if (philo->nb_philos % 2 == 1 && philo->id % 2 == 1)
		swap(&id_r, &id_l);
	if (philo->nb_philos % 2 == 0 && philo->id == philo->nb_philos)
		swap(&id_r, &id_l);
	pthread_mutex_lock(&philo->forks[id_l].mut);
	print_event(philo, "has taken a fork", get_timestamp(*philo->origin_time));
	if (philo->nb_philos == 1)
		return (my_usleep(philo, -1, get_timestamp(*philo->origin_time)),
			(void)pthread_mutex_unlock(&philo->forks[id_l].mut));
	pthread_mutex_lock(&philo->forks[id_r].mut);
	cur_time = get_timestamp(*philo->origin_time);
	print_event(philo, "has taken a fork", cur_time);
	eat2(philo, cur_time, id_l, id_r);
}
