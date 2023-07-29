/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:30:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/29 20:05:37 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	begin_eat(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	print_event(*philo, "has taken a fork");
	sem_wait(philo->sem_forks);
	print_event(*philo, "has taken a fork");
	philo->state = eating;
	sem_wait(philo->sem_last_meal);
	philo->last_meal = get_timestamp(philo->start_time);
	sem_post(philo->sem_last_meal);
	print_event(*philo, "is eating");
	usleep(philo->times[t_eat] * 1000);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);

}

static void	begin_sleep(t_philo *philo)
{
	sem_wait(philo->sem_eat_to_end);
	philo->eat_to_end--;
	sem_post(philo->sem_eat_to_end);
	philo->state = sleeping;
	print_event(*philo, "is sleeping");
	usleep(philo->times[t_sleep] * 1000);
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
		sem_wait(philo->sem_eat_to_end);
		if (philo->eat_to_end == 0)
			sem_post(philo->sem_nb_eat_to_end);
		sem_post(philo->sem_eat_to_end);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		philo->state = eating;
	while (true)
	{
		if (philo->state == thinking)
			begin_eat(philo);
		else if (philo->state == eating)
			begin_sleep(philo);
		else if (philo->state == sleeping)
		{
			philo->state = thinking;
			print_event(*philo, "is thinking");
			if (philo->nb_philos % 2 == 1)
				usleep(1000);
		}
	}
}
