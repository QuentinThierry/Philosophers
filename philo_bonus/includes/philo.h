/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:43:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/03 17:49:15 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

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

enum	e_timestamps
{
	delay,
	timestamp,
	last_meal
};

typedef struct s_philo
{
	int				id;
	int				nb_philos;
	int				eat_to_end;
	enum e_state	state;
	int				nb_forks;
	t_timeval		start_time;
	long			begin_eat;
	long			begin_sleep;
	long			last_meal;
	long			times[3];
	sem_t			*sem_forks;
	sem_t			*sem_end;
	sem_t			*sem_print;
	sem_t			*sem_eat_to_end;
}	t_philo;

// ft_atoi.c
int				ft_atoi(const char *nptr, bool *has_error);

// memory.c
void			free_philos(t_philo *philos);
void			*ft_calloc(size_t nmemb, size_t size);

// parsing.c
bool			parsing(int argc, char **argv, t_philo *philo);

// time.c
long			get_timestamp(t_timeval start_time);
long			get_time_diff(t_timeval start_time, long t_before);

#endif