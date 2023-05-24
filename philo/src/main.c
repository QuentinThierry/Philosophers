/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:38:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/24 02:43:59 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	free_forks(t_fork *forks, size_t nb_philos)
{
	size_t	i;

	i = 0;
	while (i < nb_philos)
	{
		pthread_mutex_destroy(&forks[i].mut);
		i++;
	}
	free(forks);
}

void	free_philos(t_philo *philos)
{
	if (!philos)
		return ;
	free(philos->is_end);
	pthread_mutex_destroy(philos->mut_end);
	free(philos->mut_end);
	pthread_mutex_destroy(philos->mut_print);
	free(philos->mut_print);
	pthread_mutex_destroy(philos->mut_eat_end);
	free(philos->mut_eat_end);
	pthread_mutex_destroy(&philos->forks[philos->id - 1].mut);
	free_forks(philos->forks, philos->nb_philos);
	free(philos);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;
	size_t	i;

	if (!nmemb || !size)
		return (malloc(0));
	if ((size_t)-1 / nmemb < size)
		return (NULL);
	res = malloc(nmemb * size);
	if (!res)
		return (res);
	i = 0;
	while (i < (size * nmemb))
		((char *)res)[i++] = 0;
	return (res);
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr, bool *has_error)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (!ft_isdigit(*nptr))
	{
		if (*nptr == '-')
			sign = -1;
		else if (*nptr != '+')
			return (*has_error = true, 0);
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		res = res * 10 + *nptr++ - '0';
		if (res > __INT_MAX__)
			return (*has_error = true, 0);
	}
	return (res * sign);
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

static void	refresh_time(t_timeval *before, double *timestamps)
{
	double	tmp;

	tmp = calculate_delay(before);
	timestamps[delay] += tmp;
	timestamps[timestamp] += tmp;
	timestamps[last_meal] += tmp;
}

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

bool	print_event(t_philo *philo, t_timeval *act_time,
					double *timestamps, const char *message)
{
	pthread_mutex_lock(philo->mut_end);
	if (*philo->is_end)
		return (pthread_mutex_unlock(philo->mut_end), true);
	pthread_mutex_lock(philo->mut_print);
	refresh_time(act_time, timestamps);
	printf("%-5ld %-3ld %s\n", (long)timestamps[timestamp], philo->id, message);
	pthread_mutex_unlock(philo->mut_print);
	pthread_mutex_unlock(philo->mut_end);
	return (true);
}

void	broadcast_death(t_philo	*philo, t_timeval *act_time,
						double *timestamps, bool has_print)
{
	pthread_mutex_lock(philo->mut_end);
	*philo->is_end = true;
	pthread_mutex_lock(philo->mut_print);
	refresh_time(act_time, timestamps);
	if (has_print)
		printf("%-5ld %-3ld %s\n",
			(long)timestamps[timestamp], philo->id, "is dead");
	pthread_mutex_unlock(philo->mut_print);
	pthread_mutex_unlock(philo->mut_end);
}

static enum e_state	get_next_state(enum e_state state)
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

static bool	try_take_l_fork(t_philo *philo,
							t_timeval *act_time, double *timestamps)
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
			print_event(philo, act_time, timestamps, "has taken a fork");
		}
		pthread_mutex_unlock(&philo->forks[id].mut);
	}
	return (true);
}

static bool	try_take_r_fork(t_philo *philo,
							t_timeval *act_time, double *timestamps)
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
			print_event(philo, act_time, timestamps, "has taken a fork");
		}
		pthread_mutex_unlock(&philo->forks[id].mut);
	}
	return (true);
}

bool	try_take_forks(t_philo *philo, t_timeval *act_time, double *timestamps)
{
	if (philo->nb_philos == 1)
		return (true);
	try_take_l_fork(philo, act_time, timestamps);
	try_take_r_fork(philo, act_time, timestamps);
	return (true);
}

static void	try_to_eat(t_philo *philo, t_timeval *act_time, double *timestamps)
{
	try_take_forks(philo, act_time, timestamps);
	if (philo->has_l_fork && philo->has_r_fork)
	{
		philo->state = eating;
		print_event(philo, act_time, timestamps, "is eating");
	}
}

static bool	has_all_eaten(t_philo *philo)
{
	size_t	i;

	if (philo->eat_to_end == -2)
		return (false);
	i = 0;
	pthread_mutex_lock(philo->mut_eat_end);
	while (i < philo->nb_philos)
	{
		if (philo->all_philos[i].eat_to_end > 0)
		{
			pthread_mutex_unlock(philo->mut_eat_end);
			return (false);
		}
		i++;
	}
	pthread_mutex_unlock(philo->mut_eat_end);
	return (true);
}

bool	release_forks(t_philo *philo)
{
	size_t	id;

	id = get_left_fork_id(philo);
	pthread_mutex_lock(&philo->forks[id].mut);
	philo->forks[id].is_taken = false;
	philo->has_l_fork = false;
	pthread_mutex_unlock(&philo->forks[id].mut);
	id = get_right_fork_id(philo);
	pthread_mutex_lock(&philo->forks[id].mut);
	philo->forks[id].is_taken = false;
	philo->has_r_fork = false;
	pthread_mutex_unlock(&philo->forks[id].mut);
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
		usleep(1);
		refresh_time(&act_time, timestamps);
		if (get_is_dead(philo))
			return (NULL);
		if ((long)timestamps[last_meal] > philo->times[t_die])
		{
			broadcast_death(philo, &act_time, timestamps, true);
			return (NULL);
		}
		if (philo->eat_to_end > -2 && has_all_eaten(philo))
		{
			broadcast_death(philo, &act_time, timestamps, false);
			return (NULL);
		}
		if (philo->state == thinking)
		{
			if (get_is_dead(philo))
				return (NULL);
			try_to_eat(philo, &act_time, timestamps);
			timestamps[delay] = 0;
		}
		else if (philo->state == eating
			&& timestamps[delay] > philo->times[t_eat])
		{
			if (get_is_dead(philo))
				return (NULL);
			pthread_mutex_lock(philo->mut_eat_end);
			if (philo->eat_to_end > -1)
				philo->eat_to_end--;
			pthread_mutex_unlock(philo->mut_eat_end);
			print_event(philo, &act_time, timestamps, "is sleeping");
			philo->state = get_next_state(philo->state);
			release_forks(philo);
			timestamps[last_meal] = 0;
			timestamps[delay] = 0;
		}
		else if (philo->state == sleeping
			&& timestamps[delay] > philo->times[t_sleep])
		{
			if (get_is_dead(philo))
				return (NULL);
			print_event(philo, &act_time, timestamps, "is thinking");
			philo->state = get_next_state(philo->state);
			timestamps[delay] = 0;
		}
	}
	return (NULL);
}

static bool	init_mutexs(pthread_mutex_t **mut_end, pthread_mutex_t **mut_print,
		pthread_mutex_t **mut_eat_end)
{
	*mut_end = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!*mut_end)
		return (false);
	*mut_print = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!*mut_print)
		return (free(*mut_end), false);
	*mut_eat_end = ft_calloc(1, sizeof(pthread_mutex_t));
	if (!*mut_eat_end)
		return (free(*mut_end), free(*mut_print), false);
	**mut_end = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	**mut_print = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	**mut_eat_end = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	return (true);
}

static bool	init_callocs(t_fork **forks, bool **is_end, size_t nb_philos)
{
	*forks = ft_calloc(nb_philos, sizeof(t_fork));
	*is_end = ft_calloc(1, sizeof(bool));
	if (!*forks || !*is_end)
		return (free(*forks), free(*is_end), false);
	return (true);
}

bool	fill_philo_1(t_philo *philos)
{
	size_t			i;
	pthread_mutex_t	*mut_end;
	pthread_mutex_t	*mut_print;
	pthread_mutex_t	*mut_eat_end;

	if (!init_mutexs(&mut_end, &mut_print, &mut_eat_end))
		return (false);
	i = 0;
	while (i < philos->nb_philos)
	{
		philos[i].id = i + 1;
		philos[i].state = thinking;
		philos[i].mut_end = mut_end;
		philos[i].mut_eat_end = mut_eat_end;
		philos[i].mut_print = mut_print;
		philos[i].all_philos = philos;
		i++;
	}
	return (true);
}

bool	fill_philo_2(t_philo *philos)
{
	t_fork			*forks;
	bool			*is_end;
	t_timeval		start_time;
	size_t			i;

	gettimeofday(&start_time, NULL);
	if (!init_callocs(&forks, &is_end, philos->nb_philos))
	{
		pthread_mutex_destroy(philos->mut_end);
		free(philos->mut_end);
		pthread_mutex_destroy(philos->mut_print);
		free(philos->mut_print);
		pthread_mutex_destroy(philos->mut_eat_end);
		free(philos->mut_eat_end);
		return (false);
	}
	i = 0;
	while (i < philos->nb_philos)
	{
		philos[i].is_end = is_end;
		philos[i].forks = forks;
		philos[i].start_time = start_time;
		forks[i++].mut = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	}
	return (true);
}

static void	fill_philos_value(int arg_pos, t_philo *philos, int val)
{
	size_t	j;

	j = 0;
	if (arg_pos == 1)
	{
		while (j < (size_t)val)
			philos[j++].nb_philos = val;
	}
	else if (arg_pos > 1 && arg_pos < 5)
	{
		while (j < philos->nb_philos)
			philos[j++].times[arg_pos - 2] = val;
	}
	else if (arg_pos == 5)
	{
		while (j < philos->nb_philos)
			philos[j++].eat_to_end = val;
	}
}

bool	parsing(int argc, char **argv, t_philo **philos)
{
	int		i;
	int		val;
	bool	has_error;

	if (argc > 6 || argc < 5)
		return (printf("Incorrect number of arguments.\n"), false);
	has_error = false;
	i = 1;
	while (i < argc)
	{
		val = ft_atoi(argv[i], &has_error);
		if (i == 1)
			*philos = ft_calloc(val, sizeof(t_philo));
		if (!*philos)
			return (false);
		if (has_error)
			return (printf("Invalid argument.\n"), false);
		if (val < 0)
			return (printf("Negative argument.\n"), false);
		fill_philos_value(i, *philos, val);
		i++;
	}
	if (i == 5)
		fill_philos_value(5, *philos, -2);
	return (true);
}

int	main(int argc, char **argv)
{
	t_philo		*philos;
	size_t		i;

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
	while (i < philos->nb_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	free_philos(philos);
	return (0);
}
