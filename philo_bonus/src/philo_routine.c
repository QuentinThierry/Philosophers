/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:30:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/30 17:08:26 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	begin_eat(t_philo *philo)
{
	long	cur_time;
	long	eat_time;

	sem_wait(philo->sem_forks);
	print_event(*philo, "has taken a fork", get_timestamp(philo->start_time));
	sem_wait(philo->sem_forks);
	print_event(*philo, "has taken a fork", get_timestamp(philo->start_time));

	philo->state = eating;
	cur_time = get_timestamp(philo->start_time);
	print_event(*philo, "is eating", cur_time);
	sem_wait(philo->sem_last_meal);
	philo->last_meal = cur_time;
	sem_post(philo->sem_last_meal);
	philo->last_begin_eat = cur_time;
	// eat_time = get_timestamp(philo->start_time) - cur_time;
	eat_time = cur_time;
	my_usleep(philo, philo->times[t_eat], cur_time);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
}

static void	begin_sleep(t_philo *philo)
{
	// sem_wait(philo->sem_eat_to_end);
	// philo->eat_to_end--;
	// sem_post(philo->sem_eat_to_end);
	long	cur_time;
	long	sleep_time;

	philo->state = sleeping;
	cur_time = get_timestamp(philo->start_time);
	print_event(*philo, "is sleeping", cur_time);
	// sleep_time = philo->times[t_sleep] -
		// ((cur_time - philo->last_begin_eat) - philo->times[t_sleep]);
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
		if (get_time_diff(philo->start_time, philo->last_meal)
			>= philo->times[t_die])
		{
			sem_post(philo->sem_end);
			sem_wait(philo->sem_print);
			printf("%-5ld %-3d %s\n",
				get_timestamp(philo->start_time), philo->id, "died");
			return (NULL);
		}
		sem_post(philo->sem_last_meal);
		// sem_wait(philo->sem_eat_to_end);
		// if (philo->eat_to_end == 0)
		// 	sem_post(philo->sem_nb_eat_to_end);
		// sem_post(philo->sem_eat_to_end);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		philo->state = eating;
		philo->last_begin_eat = -(philo->times[t_eat] - get_timestamp(philo->start_time));
	}
	while (true)
	{
		if (philo->state == thinking)
			begin_eat(philo);
		else if (philo->state == eating)
			begin_sleep(philo);
		else if (philo->state == sleeping)
		{
			philo->state = thinking;
			print_event(*philo, "is thinking", get_timestamp(philo->start_time));
		}
	}
}
