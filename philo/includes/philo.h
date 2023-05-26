/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:43:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/26 19:35:36 by qthierry         ###   ########.fr       */
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

typedef struct s_fork
{
	pthread_mutex_t	mut;
	bool			is_taken;
}	t_fork;

typedef struct s_philo
{
	pthread_t		thread;
	size_t			nb_philos;
	size_t			id;
	long			times[3];
	enum e_state	state;
	bool			has_l_fork;
	bool			has_r_fork;
	bool			*can_begin;
	bool			*is_end;
	pthread_mutex_t	*mut_end;
	pthread_mutex_t	*mut_print;
	pthread_mutex_t	*mut_eat_end;
	pthread_mutex_t	*mut_last_meal;
	size_t			*running_threads;
	t_timeval		*start_time;
	t_fork			*forks;
	long			last_meal;
	long			begin_eat;
	long			begin_sleep;
	long			eat_to_end;
}	t_philo;

// eat.c
void			try_to_eat(t_philo *philo);
bool			has_all_eaten(t_philo *philo);

// forks.c
bool			try_take_forks(t_philo *philo);
bool			release_forks(t_philo *philo);

// ft_atoi.c
int				ft_atoi(const char *nptr, bool *has_error);

// init.c
bool			fill_philo_1(t_philo *philos);
bool			fill_philo_2(t_philo *philos);

// memory.c
void			free_philos(t_philo *philos);
void			*ft_calloc(size_t nmemb, size_t size);

// on_death.c
void			broadcast_death(t_philo	*philo, bool has_print);

// parsing.c
bool			parsing(int argc, char **argv, t_philo **philos);

// philo_utils.c
bool			get_is_dead(t_philo *philo);
enum e_state	get_next_state(enum e_state state);
size_t			get_left_fork_id(t_philo *philo);
size_t			get_right_fork_id(t_philo *philo);

// print_event.c
bool			print_event(t_philo *philo, const char *message);

// philo_routine.c
void			*philo_routine(void *arg);

// time.c
long			get_timestamp(t_timeval start_time);
long			get_time_diff(t_timeval start_time, long t_before);

#endif