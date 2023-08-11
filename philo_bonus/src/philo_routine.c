/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:30:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/11 20:15:07 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	begin_eat(t_philo *philo)
{
	long	cur_time;

	sem_wait(philo->sem_forks);
	print_event(*philo, "has taken a fork", get_timestamp(philo->origin_time));
	sem_wait(philo->sem_forks);
	print_event(*philo, "has taken a fork", get_timestamp(philo->origin_time));
	philo->state = eating;
	cur_time = get_timestamp(philo->origin_time);
	print_event(*philo, "is eating", cur_time);
	sem_wait(philo->sem_last_meal);
	philo->last_meal = cur_time;
	sem_post(philo->sem_last_meal);
	my_usleep(philo, philo->times[t_eat], cur_time);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	philo->eat_to_end--;
}

static void	begin_sleep(t_philo *philo)
{
	long	cur_time;

	philo->state = sleeping;
	cur_time = get_timestamp(philo->origin_time);
	print_event(*philo, "is sleeping", cur_time);
	my_usleep(philo, philo->times[t_sleep], cur_time);
}

void	*philo_thread_routine(void *philo_arg)
{
	t_philo	*philo;

	philo = philo_arg;
	while (true)
	{
		usleep(500);
		sem_wait(philo->sem_last_meal);
		if (get_time_diff(philo->origin_time, philo->last_meal)
			>= philo->times[t_die])
		{
			sem_post(philo->sem_end);
			sem_wait(philo->sem_print);
			printf("%ld %d %s\n",
				get_timestamp(philo->origin_time), philo->id, "died");
			return (NULL);
		}
		sem_post(philo->sem_last_meal);
		if (philo->eat_to_end == 0)
			sem_post(philo->sem_nb_eat_to_end);
	}
	return (NULL);
}

static inline bool	begin_think(t_philo *philo)
{
	int		think_time;
	long	cur_time;

	cur_time = get_timestamp(philo->origin_time);
	print_event(*philo, "is thinking", cur_time);
	philo->state = thinking;
	think_time = 0;
	if (philo->nb_philos % 2 == 1 && philo->times[t_sleep]
		< philo->times[t_eat])
		think_time = philo->times[t_eat] - philo->times[t_sleep];
	my_usleep(philo, think_time, cur_time);
	return (true);
}

void	philo_routine(t_philo *philo)
{
	if (philo->nb_philos % 2 == 1)
		if (philo->id % 2 == 1)
			usleep(1000);
	if (philo->id % 2 == 0)
		philo->state = eating;
	while (true)
	{
		if (philo->state == thinking)
			begin_eat(philo);
		else if (philo->state == eating)
			begin_sleep(philo);
		else if (philo->state == sleeping)
			begin_think(philo);
	}
}
