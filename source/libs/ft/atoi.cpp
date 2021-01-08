/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 14:28:52 by atetu             #+#    #+#             */
/*   Updated: 2021/01/08 14:43:48 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libs/ft.hpp>

namespace ft
{
    int	atoi(const char *str)
    {
        unsigned int		nb;
        int					negative;

        nb = 0;
        negative = 1;
        while (*str && (*str == '\t' || *str == '\n' || *str == '\v' ||
            *str == '\f' || *str == '\r' || *str == ' '))
            str++;
        if (*str == '+' || *str == '-')
        {
            if (*str == '-')
                negative = -1;
            str++;
        }
        while ((*str >= 48 && *str <= 57) && *str)
        {
            nb = nb * 10 + (*str - 48);
            str++;
        }
        return ((int)(nb * negative));
    }
}