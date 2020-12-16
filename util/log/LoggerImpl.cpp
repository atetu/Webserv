/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoggerImpl.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 10:54:02 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/20 10:54:02 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/Enum.hpp>
#include <util/log/LoggerImpl.hpp>
#include <util/log/LogLevel.hpp>

const std::string LoggerImpl::DEFAULT_TAG = "???";
NullStream LoggerImpl::VOID;

LoggerImpl::LoggerImpl() :
		m_tag(DEFAULT_TAG)
{
}

LoggerImpl::LoggerImpl(const std::string &tag) :
		m_tag(tag)
{
}

LoggerImpl::LoggerImpl(const LoggerImpl &other) :
		m_tag(other.m_tag)
{
}

LoggerImpl::~LoggerImpl()
{
}

std::ostream&
LoggerImpl::trace() const
{
	return (log(*LogLevel::TRACE));
}

std::ostream&
LoggerImpl::debug() const
{
	return (log(*LogLevel::DEBUG));
}

std::ostream&
LoggerImpl::info() const
{
	return (log(*LogLevel::INFO));
}

std::ostream&
LoggerImpl::warn() const
{
	return (log(*LogLevel::WARN));
}

std::ostream&
LoggerImpl::error() const
{
	return (log(*LogLevel::ERROR));
}

std::ostream&
LoggerImpl::fatal() const
{
	return (log(*LogLevel::FATAL));
}

std::ostream&
LoggerImpl::log(LogLevel &level) const
{
	if (LogLevel::ACTIVE == NULL || LogLevel::ACTIVE->ordinal() > level.ordinal())
		return (VOID);

	return (std::cout << "[" << level.name() << "] " << m_tag << ": ");
}

const std::string&
LoggerImpl::tag() const
{
	return (m_tag);
}
