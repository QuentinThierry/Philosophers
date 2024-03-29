/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:43:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/26 19:44:54 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/types.h>

typedef struct timeval	t_timeval;

enum	e_state
{
	eating,
	sleeping,
	thinking
};

enum	e_times
{
	t_die,
	t_eat,
	t_sleep
};

typedef struct s_philo
{
	int				id;
	int				nb_philos;
	int				eat_to_end;
	int				nb_actions;
	enum e_state	state;
	t_timeval		origin_time;
	long			last_meal;
	long			times[3];
	pthread_t		thread;
	sem_t			*sem_forks;
	sem_t			*sem_end;
	sem_t			*sem_print;
	sem_t			*sem_nb_eat_to_end;
	sem_t			*sem_last_meal;
}	t_philo;

// ft_atoi.c
int				ft_atoi(const char *nptr, bool *has_error);
char			*ft_itoa(int n);
char			*ft_strjoin(char const *s1, char const *s2);

// memory.c
void			free_philos(t_philo *philos);
void			*ft_calloc(size_t nmemb, size_t size);

// parsing.c
bool			parsing(int argc, char **argv, t_philo *philo);

// time.c
void			my_usleep(t_philo *philo, long delay, long start_time);
long			get_timestamp(t_timeval start_time);
long			get_time_diff(t_timeval start_time, long t_before);

// sem_handle.c
void			close_semaphores(t_philo *philo);
bool			open_semaphores(t_philo *philo);
bool			create_semaphore_threads(t_philo *philo);
void			destroy_semaphore_threads(t_philo *philo);

// utils.c
void			print_event(t_philo philo, const char *message, long time);
bool			init(int argc, char **argv, t_philo *philo, pid_t **pids);
void			kill_all_philos(t_philo philo, pid_t *pids);

//philo_routine.c
void			philo_routine(t_philo *philo);
void			*philo_thread_routine(void *philo);

#endif