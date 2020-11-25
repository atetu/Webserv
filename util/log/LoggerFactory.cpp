/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoggerFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 16:06:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 16:06:47 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/log/LoggerFactory.hpp>
#include <util/log/LoggerImpl.hpp>
#include <utility>

Logger&
LoggerFactory::get(const std::string &tag)
{
	logger_map &storage = getStorage();

	Logger *current = storage[tag];

	if (!current)
	{
		current = new LoggerImpl(tag);

		storage.insert(storage.end(), std::pair<std::string, Logger*>(tag, current));
	}

	return (*current);
}

void
LoggerFactory::destroy(void)
{
	logger_map &storage = getStorage();

	for (iterator it = storage.begin(); it != storage.begin(); it++)
		delete it->second;

	storage.clear();
}

LoggerFactory::logger_map&
LoggerFactory::getStorage()
{
	static logger_map storage;

	return (storage);
}
