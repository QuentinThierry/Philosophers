/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:47:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/03 17:46:33 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_event(t_philo philo, const char *message)
{
	sem_wait(philo.sem_print);
	printf("%-5ld %-3d %s\n",
		get_timestamp(philo.start_time), philo.id, message);
	sem_post(philo.sem_print);
}

// returns 0 if the philo died, else 1
void	philo_routine(t_philo philo)
{
	int	nb_eat;

	nb_eat = 0;
	while (true)
	{
		usleep(100);
		if (nb_eat == philo.eat_to_end)
		{
			sem_post(philo.sem_eat_to_end);
			nb_eat++;
		}
		if (get_time_diff(philo.start_time, philo.last_meal)
			>= philo.times[t_die])
		{
			sem_wait(philo.sem_print);
			sem_post(philo.sem_end);
			printf("%-5ld %-3d %s\n",
				get_timestamp(philo.start_time), philo.id, "is dead");
			return ;
		}
		if (philo.state == thinking)
		{
			sem_wait(philo.sem_forks);
			philo.nb_forks++;
			print_event(philo, "has taken a fork");
			if (philo.nb_forks == 2)
			{
				print_event(philo, "is eating");
				philo.state = eating;
				philo.begin_eat = get_timestamp(philo.start_time);
			}
		}
		else if (philo.state == eating
			&& get_time_diff(philo.start_time, philo.begin_eat)
			>= philo.times[t_eat])
		{
			philo.last_meal = get_timestamp(philo.start_time);
			philo.begin_sleep = get_timestamp(philo.start_time);
			sem_post(philo.sem_forks);
			sem_post(philo.sem_forks);
			nb_eat++;
			philo.state = sleeping;
			philo.nb_forks = 0;
			print_event(philo, "is sleeping");
		}
		else if (philo.state == sleeping
			&& get_time_diff(philo.start_time, philo.begin_sleep)
			>= philo.times[t_sleep])
		{
			philo.state = thinking;
			print_event(philo, "is thinking");
		}
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

void	close_semaphores(t_philo *philo)
{
	if (philo)
	{
		if (philo->sem_end)
			sem_close(philo->sem_end);
		if (philo->sem_forks)
			sem_close(philo->sem_forks);
		if (philo->sem_print)
			sem_close(philo->sem_print);
		if (philo->sem_eat_to_end)
			sem_close(philo->sem_eat_to_end);
	}
	sem_unlink("philo_end");
	sem_unlink("philo_forks");
	sem_unlink("philo_print");
	sem_unlink("philo_eat_to_end");
}

bool	open_semaphores(t_philo *philo)
{
	const char	*error = "Error opening semaphore";

	philo->sem_forks = sem_open("philo_forks", O_CREAT, 0664, philo->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_end = sem_open("philo_end", O_CREAT, 0664, 0);
	if (philo->sem_end == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_print = sem_open("philo_print", O_CREAT, 0664, 1);
	if (philo->sem_print == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_eat_to_end = sem_open("philo_eat_to_end", O_CREAT, 0664, 0);
	if (philo->sem_eat_to_end == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	return (true);
}

int	end_checker(t_philo philo, pid_t *pids)
{
	int		i;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (printf("Error creating fork\n"), sem_post(philo.sem_end), 0);
	if (pid == 0)
	{
		free(pids);
		i = 0;
		while (i < philo.nb_philos)
		{
			sem_wait(philo.sem_eat_to_end);
			i++;
		}
		sem_post(philo.sem_end);
		sem_close(philo.sem_end);
		sem_close(philo.sem_forks);
		sem_close(philo.sem_print);
		sem_close(philo.sem_eat_to_end);
		return (1);
	}
	return (0);
}

void	kill_all_philos(t_philo philo, pid_t *pids)
{
	int	i;

	i = 0;
	close_semaphores(&philo);
	while (i < philo.nb_philos)
	{
		if (pids[i] != 0)
			kill(pids[i], SIGKILL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t		*pids;
	t_philo		philo;
	int			value;
	t_timeval	time;

	close_semaphores(NULL);
	philo = (t_philo){0};
	if (!parsing(argc, argv, &philo))
		return (1);
	pids = ft_calloc(philo.nb_philos, sizeof(pid_t));
	if (!pids)
		return (1);
	if (!open_semaphores(&philo))
		return (free(pids), 1);
	gettimeofday(&time, NULL);
	value = create_philos_processes(philo.nb_philos, pids, &philo);
	if (value < 0)
		return (kill_all_philos(philo, pids), free(pids), 0);
	if (value > 0)
	{
		free(pids);
		philo.start_time = time;
		philo.last_meal = get_timestamp(time);
		if (philo.id % 2 == 0)
			usleep(50000);
		philo_routine(philo);
	}
	else
	{
		if (end_checker(philo, pids))
			return (0);
		sem_wait(philo.sem_end);
		kill_all_philos(philo, pids);
		free(pids);
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	close_semaphores(&philo);
	return (0);
}
