/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/25 22:10:18 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static inline bool	begin_sleep(t_philo *philo,
		t_timeval *act_time, double *timestamps)
{
	if (get_is_dead(philo))
		return (false);
	pthread_mutex_lock(philo->mut_last_meal);
	philo->last_meal = get_timestamp(philo->start_time);
	pthread_mutex_unlock(philo->mut_last_meal);
	pthread_mutex_lock(philo->mut_eat_end);
	if (philo->eat_to_end > -1)
		philo->eat_to_end--;
	pthread_mutex_unlock(philo->mut_eat_end);
	print_event(philo, act_time, timestamps, "is sleeping");
	philo->begin_sleep = get_timestamp(philo->start_time);
	philo->state = get_next_state(philo->state);
	release_forks(philo);
	return (true);
}

static inline bool	begin_think(t_philo *philo,
		t_timeval *act_time, double *timestamps)
{
	if (get_is_dead(philo))
		return (false);
	print_event(philo, act_time, timestamps, "is thinking");
	philo->state = get_next_state(philo->state);
	return (true);
}

static inline bool	philo_routine2(t_philo *philo,
	t_timeval *act_time, double *timestamps)
{
	// printf("last sleep : %ld \n", get_time_diff(philo->start_time, philo->begin_sleep));
	if (philo->state == thinking)
	{
		try_to_eat(philo, act_time, timestamps);
	}
	else if (philo->state == eating
		&& get_time_diff(philo->start_time, philo->begin_eat) >= philo->times[t_eat])
	{
		if (!begin_sleep(philo, act_time, timestamps))
			return (false);
	}
	else if (philo->state == sleeping
		&& get_time_diff(philo->start_time, philo->begin_sleep) >= philo->times[t_sleep])
	{
		if (!begin_think(philo, act_time, timestamps))
			return (false);
	}
	return (true);
}

void	*philo_routine(void *arg)
{
	t_timeval	act_time;
	t_philo		*philo;
	double		*timestamps;

	timestamps = (double [3]){0, 0, 0};
	philo = arg;
	act_time = philo->start_time;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (true)
	{
		usleep(10);
		if (get_is_dead(philo))
			return (false);
		if (!philo_routine2(philo, &act_time, timestamps))
			return (NULL);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo		*philos;
	size_t		i;
	bool		end;

	if (!parsing(argc, argv, &philos))
		return (false);
	if (!(fill_philo_1(philos) && fill_philo_2(philos)))
		return (free(philos), printf("Memory exhausted\n"), 1);
	i = 0;
	while (i < philos->nb_philos)
	{
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	i = 0;
	end = false;
	while (!end)
	{
		usleep(1000);
		i = 0;
		// if (philos->eat_to_end != -2 && has_all_eaten(philos))
		// {
		// 	broadcast_death(philos, false);
		// 	break ;
		// }
		pthread_mutex_lock(philos->mut_last_meal);
		while (i < philos->nb_philos)
		{
			if (get_timestamp(philos[i].start_time) - philos[i].last_meal >= philos->times[t_die])
			{
				broadcast_death(&philos[i], get_timestamp(philos[i].start_time), true);
				end = true;
				break ;
			}
			i++;
		}
		pthread_mutex_unlock(philos->mut_last_meal);
	}
	i = 0;
	while (i < philos->nb_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	free_philos(philos);
	return (0);
}
