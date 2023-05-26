/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:47:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/26 22:53:05 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_event(t_philo philo, const char *message)
{
	printf("%-5ld %-3d %s\n",
		get_timestamp(philo.start_time), philo.id, message);
}

void	philo_routine(t_philo philo)
{
	while (true)
	{
		print_event(philo, "test");
		// if (philo.state == thinking)
		// {
		// 	sem_wait(philo.sem_forks);
		// 	philo.nb_forks++;
		// 	print_event(philo, "has taken a fork");
		// 	if (philo.nb_forks == 2)
		// 	{
		// 		print_event(philo, "is eating");
		// 		philo.state = eating;
		// 		philo.begin_eat = get_timestamp(philo.start_time);
		// 	}
		// }
		// else if (philo.state == eating
		// 	&& get_time_diff(philo.start_time, philo.begin_eat)
		// 	>= philo.times[t_eat])
		// {
		// 	sem_post(philo.sem_forks);
		// 	sem_post(philo.sem_forks);
		// 	philo.state = sleeping;
		// 	philo.begin_sleep = get_timestamp(philo.start_time);
		// 	philo.nb_forks = 0;
		// 	print_event(philo, "is sleeping");
		// }
		// else if (philo.state == sleeping
		// 	&& get_time_diff(philo.start_time, philo.begin_sleep)
		// 	>= philo.times[t_sleep])
		// {
		// 	philo.state = thinking;
		// 	philo.last_meal = get_timestamp(philo.start_time);
		// 	print_event(philo, "is thinking");
		// }
	}
}

// returns -1 for error, 0 for main and 1 for child
int	create_philos_processes(int nb_philos, pid_t *pids, t_philo *philo)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < nb_philos)
	{
		pid = fork();
		if (pid < 0) //do something on error
			return (-1);
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

bool	open_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open("forks", O_CREAT, 0664, philo->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (printf("Error opening semaphore\n"), false);
	return (true);
}



// void	print_pids(pid_t *pids)
// {
// 	int		i;

// 	i = 0;
// 	while (i < NB_PHILOS)
// 	{
// 		printf("pid : %d\n", pids[i]);
// 		i++;
// 	}
// 	printf("main pid : %d\n", getpid());
// }

int	main(int argc, char **argv)
{
	pid_t		*pids;
	t_philo		philo;
	int			value;
	t_timeval	time;

	sem_unlink("forks");
	philo = (t_philo){0};
	if (!parsing(argc, argv, &philo))
		return (1);
	pids = ft_calloc(philo.nb_philos, sizeof(pid_t));
	if (!pids)
		return (1);
	if (!open_semaphores(&philo))
	{
		free(pids);
		return (1);
	}
	printf("nb philos : %d\n", philo.nb_philos);
	gettimeofday(&time, NULL);
	value = create_philos_processes(philo.nb_philos, pids, &philo);
	if (value > 0)
	{
		philo.start_time = time;
		if (philo.id % 2 == 0)
			usleep(5000);
		philo_routine(philo);
	}
	
	while (waitpid(-1, NULL, 0) > 0)
		;
	free(pids);
	// printf("end child : %d\n", getpid());
	sem_close(philo.sem_forks);
	sem_unlink("forks");
}
