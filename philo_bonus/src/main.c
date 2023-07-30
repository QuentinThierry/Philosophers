/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:47:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/30 15:04:27 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// returns -1 for error, 0 for main and 1 for child
static int	create_philos_processes(int nb_philos, pid_t *pids, t_philo *philo)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < nb_philos)
	{
		pid = fork();
		if (pid < 0)
			return (printf("Error creating fork\n"), -1);
		if (pid == 0)
		{
			philo->id = i + 1;
			return (1);
		}
		pids[i] = pid;
		i++;
	}
	return (0);
}

static void	kill_all_philos(t_philo philo, pid_t *pids)
{
	int	i;

	i = 0;
	close_semaphores(&philo);
	while (i < philo.nb_philos + 1)
	{
		if (pids[i] != 0)
			kill(pids[i], SIGKILL);
		i++;
	}
}

static bool	create_semaphore_mutex(t_philo *philo)
{
	const char	*error = "Error opening semaphore";

	philo->sem_eat_to_end = sem_open("/philo_eat_to_end", O_CREAT, 0664, 1);
	if (philo->sem_eat_to_end == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_last_meal = sem_open("/philo_last_meal", O_CREAT, 0664, 1);
	if (philo->sem_last_meal == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	return (true);
}

static bool	create_thread(t_philo *philo)
{
	if (pthread_create(&philo->thread, NULL, philo_thread_routine, philo) != 0)
		return (printf("Error on creating thread\n"), false);
	pthread_detach(philo->thread);
	return (true);
}

bool	exec_process(t_philo *philo, int value, pid_t *pids, t_timeval time)
{
	if (value > 0)
	{
		free(pids);
		philo->start_time = time;
		philo->last_meal = get_timestamp(time);
		if (!create_semaphore_mutex(philo))
			return (false);
		create_thread(philo);
		philo_routine(philo);
	}
	else
	{
		sem_wait(philo->sem_end);
		kill_all_philos(*philo, pids);
		free(pids);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	pid_t		*pids;
	t_philo		philo;
	int			value;
	t_timeval	time;

	if (!init(argc, argv, &philo, &pids))
		return (1);
	gettimeofday(&time, NULL);
	value = create_philos_processes(philo.nb_philos, pids, &philo);
	if (value < 0)
		return (kill_all_philos(philo, pids), free(pids), 0);
	if (!exec_process(&philo, value, pids, time))
		return (0);
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (0);
}
