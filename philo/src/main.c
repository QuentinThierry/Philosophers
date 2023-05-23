/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/23 02:21:12 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

#define T_EAT 100
#define T_SLEEP 100
#define T_DIE 2000

static bool	get_is_dead(t_philo *philo)
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

bool	print_event(t_philo *philo, double timestamp, const char *message)
{
	if (get_is_dead(philo))
		return (true);
	printf("%-5ld %-3ld %s\n", (long)timestamp, philo->id, message); // secure w mut ?
	return (true);
}

static double	calculate_delay(t_timeval *before)
{
	t_timeval	time;
	double		delay;

	gettimeofday(&time, NULL);
	delay = (time.tv_usec - before->tv_usec)
		+ (time.tv_sec - before->tv_sec) * CLOCKS_PER_SEC;
	*before = time;
	return (delay / 1000);
}

static enum e_state get_next_state(enum e_state state)
{
	if (state == thinking)
		return (eating);
	return (state + 1);
}

static size_t	get_left_fork_id(t_philo *philo)
{
	if (philo->id == philo->nb_philos)
		return (0);
	return (philo->id);
}

static size_t	get_right_fork_id(t_philo *philo)
{
	return (philo->id - 1);
}

static bool	try_take_l_fork(t_philo *philo, double timestamp)
{
	size_t	id;

	if (!philo->has_l_fork)
	{
		id = get_left_fork_id(philo);
		pthread_mutex_lock(&philo->forks[id].mut);
		if (!philo->forks[id].is_taken)
		{
			philo->forks[id].is_taken = true;
			philo->has_l_fork = true;
			print_event(philo, timestamp, "has taken a left fork");
		}
		pthread_mutex_unlock(&philo->forks[id].mut);
	}
	return (true);
}

static bool	try_take_r_fork(t_philo *philo, double timestamp)
{
	size_t	id;

	if (!philo->has_r_fork)
	{
		id = get_right_fork_id(philo);
		pthread_mutex_lock(&philo->forks[id].mut);
		if (!philo->forks[id].is_taken)
		{
			philo->forks[id].is_taken = true;
			philo->has_r_fork = true;
			print_event(philo, timestamp, "has taken a right fork");
		}
		pthread_mutex_unlock(&philo->forks[id].mut);
	}
	return (true);
}

bool	try_take_forks(t_philo *philo, double timestamp)
{
	if (philo->nb_philos == 1)
		return (true);
	try_take_l_fork(philo, timestamp);
	try_take_r_fork(philo, timestamp);
	return (true);
}

static void	try_to_eat(t_philo *philo, double timestamp)
{
	try_take_forks(philo, timestamp);
	if (philo->has_l_fork && philo->has_r_fork)
	{
		philo->state = eating;
		print_event(philo, timestamp, "is eating");
	}
}

void	broadcast_death(t_philo	*philo, double timestamp)
{
	t_philo	*philos;

	philos = philo->all_philos;
	pthread_mutex_lock(philos->mut_end);
	*philos->is_end = true;
	printf("%-5ld %-3ld %s\n", (long)timestamp, philo->id, "is dead");
	pthread_mutex_unlock(philos->mut_end);
}

bool	release_forks(t_philo *philo)
{
	size_t	id;

	id = get_left_fork_id(philo);
	pthread_mutex_lock(&philo->forks[id].mut);
	philo->forks[id].is_taken = false;
	pthread_mutex_unlock(&philo->forks[id].mut);
	id = get_right_fork_id(philo);
	pthread_mutex_lock(&philo->forks[id].mut);
	philo->forks[id].is_taken = false;
	pthread_mutex_unlock(&philo->forks[id].mut);
	philo->has_l_fork = false;
	philo->has_r_fork = false;
	return (true);
}

void	*philo_routine(void *arg)
{
	double		delay = 0;
	double		last_meal = 0;
	double		timestamp = 0;
	double		tmp = 0;
	t_timeval	time;
	t_philo		*philo;
	
	philo = arg;
	gettimeofday(&time, NULL);
	delay += calculate_delay(&time);
	while (true)
	{
		usleep(1);
		tmp = calculate_delay(&time);
		timestamp += tmp;
		delay += tmp;
		last_meal += tmp;
		if (get_is_dead(philo))
			return (NULL);
		if ((long)last_meal >= philo->times[t_die])
		{
			broadcast_death(philo->all_philos, timestamp);
			return (NULL);
		}
		if (philo->state == thinking)
		{
			if (get_is_dead(philo))
				return (NULL);
			try_to_eat(philo, timestamp);
			delay = 0;
		}
		else if (philo->state == eating && (long)delay >= philo->times[t_eat])
		{
			print_event(philo, timestamp, "is sleeping");
			if (get_is_dead(philo))
				return (NULL);
			philo->state = get_next_state(philo->state);
			release_forks(philo);
			last_meal = 0;
			delay = 0;
		}
		else if (philo->state == sleeping && (long)delay >= philo->times[t_sleep])
		{
			if (get_is_dead(philo))
				return (NULL);
			philo->state = get_next_state(philo->state);
			print_event(philo, timestamp, "is thinking");
			delay = 0;
		}
	}
	return (NULL);
}

bool	init_philos(t_philo *philos, size_t nb_philos)
{
	size_t			i;
	t_fork			*forks;
	bool			*is_end;
	pthread_mutex_t	*mut_end;

	forks = calloc(nb_philos, sizeof(t_fork)); //calloc
	if (!forks)
		return (false);
	is_end = calloc(1, sizeof(bool));
	if (!is_end)
		return (false);
	mut_end = calloc(1, sizeof(pthread_mutex_t));
	if (!mut_end)
		return (false);
	*mut_end = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	i = 0;
	while (i < nb_philos)
	{
		philos[i].is_end = is_end;
		philos[i].nb_philos = nb_philos;
		philos[i].all_philos = philos;
		philos[i].id = i + 1;
		philos[i].state = thinking;
		philos[i].times[0] = T_EAT;
		philos[i].times[1] = T_SLEEP;
		philos[i].times[2] = T_DIE;
		philos[i].forks = forks;
		philos[i].mut_end = mut_end;
		forks[i].mut = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
		i++;
	}
	return (true);
}

#define NB_PHILOS 20

int main()
{
	t_philo		*philos;
	size_t		i;

	philos = calloc(NB_PHILOS, sizeof(t_philo));
	if (!philos)
		return (1);
	if (!init_philos(philos, NB_PHILOS))
		return (1);
	i = 0;
	while (i < philos->nb_philos)
	{
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	i = 0;
	while (i < philos->nb_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}

	return (0);
}