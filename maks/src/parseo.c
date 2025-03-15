/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarinov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:36:32 by mmarinov          #+#    #+#             */
/*   Updated: 2025/02/18 13:33:06 by mmarinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	ft_atoi(const char *s)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		res = res * 10 + (*s - '0');
		if (res > INT_MAX)
			return (-2);
		s++;
	}
	if (*s != '\0')
		return (-1);
	return ((int)res * sign);
}

int	check_int_count(int ac)
{
	if (ac != 5 && ac != 6)
	{
		printf(RED"Wrong input!\n"RES
			GR"Correct is (EX) : ./philo 4 800 200 200 [4]\n"RES);
		return (0);
	}
	return (1);
}

int	check_num_filo(char *arg)
{
	int	param;

	param = ft_atoi(arg);
	if (param > 200)
	{
		printf(RED"Number of Philos is too big!(max = 200)\n"RES);
		return (0);
	}
	return (1);
}

int	check_params(char *arg)
{
	int	param;

	param = ft_atoi(arg);
	if (param == -1)
	{
		printf(RED"ERROR: The number contains invalid characters!\n"RES);
		return (0);
	}
	if (param == -2)
	{
		printf(RED"ERROR: Number is too big!\n"RES);
		return (0);
	}
	if (param <= 0)
	{
		printf(RED"ERROR: Params should not be zero or negative numbers\n"RES);
		return (0);
	}
	return (1);
}
