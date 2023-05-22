/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/22 03:33:44 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

#define NB_PHILOS 1
#define T_EAT 10
#define T_SLEEP 10
#define T_DIE 20

static const char	*state_to_char(enum e_state state)
{
	if (state == eating)
		return ("Eating");
	if (state == sleeping)
		return ("Sleeping");
	if (state == thinking)
		return ("Thinking");
	return ("Unreachable");
}

static long	calculate_delay(t_timeval *before)
{
	t_timeval	time;
	long		delay;

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

static void	try_to_eat(t_philo *philo, long *last_meal)
{
	// if (!philo->has_l_fork || !philo->has_l_fork)
	// 	return ;
	philo->state = eating;
}

void	*philo_routine(void *arg)
{
	long		delay = 0;
	long		last_meal = 0;
	long		timestamp = 0;
	long		tmp = 0;
	t_timeval	time;
	t_philo		*philo;
	
	philo = arg;
	gettimeofday(&time, NULL);
	delay += calculate_delay(&time);
	while (true)
	{
		tmp = calculate_delay(&time);
		delay += tmp;
		last_meal += tmp;
		printf("last_meal = %ld + state : %s + delay : %ld + tmp %ld\n",
			last_meal, state_to_char(philo->state), delay, tmp);
		if (last_meal >= philo->times[t_die])
			return (philo->is_dead = true, NULL);
		if (philo->state == thinking)
			try_to_eat(philo, &last_meal);
		else if (philo->state == eating && delay >= philo->times[t_eat])
		{
			printf("eat!\n");
			philo->state = get_next_state(philo->state);
			last_meal = 0;
			timestamp += delay;
			delay = 0;
		}
		else if (philo->state == sleeping && delay >= philo->times[t_sleep])
		{
			philo->state = get_next_state(philo->state);
			timestamp += delay;
			delay = 0;
		}
	}
	return (NULL);
}

bool	init_philos(t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < NB_PHILOS)
	{
		philos[i].id = i + 1;
		philos[i].state = thinking;
		philos[i].times[0] = T_EAT;
		philos[i].times[1] = T_SLEEP;
		philos[i].times[2] = T_DIE;
		philos[i].has_l_fork = 1;
		philos[i].has_r_fork = 1;
		i++;
	}
	return (true);
}


int main()
{
	t_philo	*philos;
	size_t		i;

	philos = calloc(NB_PHILOS, sizeof(t_philo));
	if (!philos)
		return (1);
	if (!init_philos(philos))
		return (1);
	
	i = 0;
	while (i < NB_PHILOS)
	{
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
	i = 0;
	while (i < NB_PHILOS)
	{
		pthread_join(philos[i].thread, NULL);
		if (philos[i].is_dead)
		{
			printf("MORT!\n");
		}
		else
			printf("VIVANT!\n");
		i++;
	}

	return (0);
}