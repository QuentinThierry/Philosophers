/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:30:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/29 16:36:13 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	begin_eat(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	philo->nb_forks++;
	print_event(*philo, "has taken a fork");
	if (philo->nb_forks == 2)
	{
		philo->last_meal = get_timestamp(philo->start_time);
		print_event(*philo, "is eating");
		philo->state = eating;
		philo->begin_eat = get_timestamp(philo->start_time);
	}
}

static void	begin_sleep(t_philo *philo, int *nb_eat)
{
	philo->begin_sleep = get_timestamp(philo->start_time);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	(*nb_eat)++;
	philo->state = sleeping;
	philo->nb_forks = 0;
	print_event(*philo, "is sleeping");
}

static bool	check_for_end(t_philo *philo, int *nb_eat)
{
	if (*nb_eat == philo->eat_to_end)
	{
		sem_post(philo->sem_eat_to_end);
		(*nb_eat)++;
	}
	if (get_time_diff(philo->start_time, philo->last_meal)
		>= philo->times[t_die])
	{
		sem_wait(philo->sem_print);
		sem_post(philo->sem_end);
		printf("%-5ld %-3d %s\n",
			get_timestamp(philo->start_time), philo->id, "is dead");
		return (false);
	}
	return (true);
}

// returns 0 if the philo died, else 1
void	philo_routine(t_philo philo)
{
	int	nb_eat;

	nb_eat = 0;
	// sem_wait(philo.sem_start_all);
	while (true)
	{
		usleep(1000);
		if (!check_for_end(&philo, &nb_eat))
			return ;
		if (philo.state == thinking)
			begin_eat(&philo);
		else if (philo.state == eating
			&& get_time_diff(philo.start_time, philo.begin_eat)
			>= philo.times[t_eat])
			begin_sleep(&philo, &nb_eat);
		else if (philo.state == sleeping
			&& get_time_diff(philo.start_time, philo.begin_sleep)
			>= philo.times[t_sleep])
		{
			philo.state = thinking;
			print_event(philo, "is thinking");
		}
	}
}
