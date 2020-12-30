/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogLevel.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 16:09:19 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 16:09:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/log/LogLevel.hpp>

#define DEFINE_LEVEL(name) ENUM_DEFINE(LogLevel, name, LogLevel());

DEFINE_LEVEL(TRACE);
DEFINE_LEVEL(DEBUG);
DEFINE_LEVEL(INFO);
DEFINE_LEVEL(WARN);
DEFINE_LEVEL(ERROR);
DEFINE_LEVEL(FATAL);

const LogLevel *LogLevel::ACTIVE = NULL;
