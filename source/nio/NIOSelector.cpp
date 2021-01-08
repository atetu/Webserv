/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 23:38:08 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/07 23:38:08 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <nio/NIOSelector.hpp>
#include <sys/select.h>
#include <util/Collections.hpp>
#include <util/System.hpp>
#include <iostream>
#include <string>

Logger &NIOSelector::LOG = LoggerFactory::get("NIO Selector");

NIOSelector::NIOSelector() :
		m_fds(),
		m_read(),
		m_write(),
		m_highest(),
		m_operationConfig(),
		m_callbacks(),
		m_fileDescriptors()
{
}

NIOSelector::NIOSelector(const NIOSelector &other) :
		m_fds(other.m_fds),
		m_read(other.m_read),
		m_write(other.m_write),
		m_highest(other.m_highest),
		m_operationConfig(other.m_operationConfig),
		m_callbacks(other.m_callbacks),
		m_fileDescriptors(other.m_fileDescriptors)
{
}

NIOSelector::~NIOSelector()
{
}

NIOSelector&
NIOSelector::operator =(const NIOSelector &other)
{
	if (this != &other)
	{
		m_fds = other.m_fds;
		m_read = other.m_read;
		m_write = other.m_write;
		m_highest = other.m_highest;
		m_operationConfig = other.m_operationConfig;
		m_callbacks = other.m_callbacks;
		m_fileDescriptors = other.m_fileDescriptors;
	}

	return (*this);
}

void
NIOSelector::add(FileDescriptor &fd, Callback &callback, int operations)
{
	int raw = fd.raw();

	addToSets(fd, operations);

	m_callbacks[raw] = &callback;
	m_fileDescriptors[raw] = &fd;

	if (LOG.isTraceEnabled())
		LOG.trace() << "Added: " << raw << "  (opts=" << operations << ")" << std::endl;
}

void
NIOSelector::update(FileDescriptor &fd, int operations)
{
	int opts = this->operations(fd);

	if (removeFromSets(fd))
		addToSets(fd, operations);

	if (LOG.isTraceEnabled())
		LOG.trace() << "Updated: " << fd.raw() << "  (from opts= " << opts << ", to opts=" << operations << ")" << std::endl;
}

void
NIOSelector::remove(FileDescriptor &fd)
{
	int opts = operations(fd);

	if (removeFromSets(fd))
	{
		int raw = fd.raw();

		Collections::remove(m_operationConfig, raw);
		Collections::remove(m_callbacks, raw);
		Collections::remove(m_fileDescriptors, raw);

		if (LOG.isTraceEnabled())
			LOG.trace() << "Removed: " << fd.raw() << "  (opts=" << opts << ")" << std::endl;
	}
	else
	{
		if (LOG.isTraceEnabled())
			LOG.trace() << "Not Removed: " << fd.raw() << std::endl;
	}
}

int
NIOSelector::select(FileDescriptorSet *read, FileDescriptorSet *write, struct timeval *timeout)
{
	fd_set *readSet = NULL;
	fd_set *writeSet = NULL;

	if (read)
	{
		*read = m_read;
		readSet = &read->storage();
	}

	if (write)
	{
		*write = m_write;
		writeSet = &write->storage();
	}

	int r = ::select(m_highest + 1, readSet, writeSet, NULL, timeout);

	if (r > 0)
	{
		if (read)
			for (int index = 0; index <= m_highest; index++)
			{
				if (read->test(index))
					m_callbacks[index]->readable(*m_fileDescriptors[index]);
			}

		if (write)
			for (int index = 0; index <= m_highest; index++)
			{
				if (write->test(index))
					m_callbacks[index]->writable(*m_fileDescriptors[index]);
			}
	}

	return (r);
}

int
NIOSelector::operations(FileDescriptor &fd) const
{
	OperationConfigMap::const_iterator it = m_operationConfig.find(fd.raw());
	if (it == m_operationConfig.end())
		return (-1);

	return (it->second);
}

const FileDescriptorSet&
NIOSelector::fds() const
{
	return (m_fds);
}

const FileDescriptorSet&
NIOSelector::readFds() const
{
	return (m_read);
}

const FileDescriptorSet&
NIOSelector::writeFds() const
{
	return (m_write);
}

void
NIOSelector::addToSets(FileDescriptor &fd, int operations)
{
	int raw = fd.raw();

	if (operations & (ACCEPT | READ))
		m_read.set(raw);

	if (operations & (WRITE))
		m_write.set(raw);

	m_fds.set(raw);

	if (raw > m_highest)
		m_highest = raw;

	m_operationConfig[raw] = operations;
}

bool
NIOSelector::removeFromSets(FileDescriptor &fd)
{
	int opts = operations(fd);

	if (opts != -1)
	{
		int raw = fd.raw();

		if (opts)
		{
			if (opts & (ACCEPT | READ))
				m_read.clear(raw);

			if (opts & (WRITE))
				m_write.clear(raw);
		}

		m_fds.clear(raw);

		if (raw == m_highest)
			m_highest--;

		return (true);
	}

	return (false);
}

NIOSelector::Callback::~Callback()
{
}

bool
NIOSelector::Callback::readable(FileDescriptor&)
{
	return (false);
}

bool
NIOSelector::Callback::writable(FileDescriptor&)
{
	return (false);
}

void
NIOSelector::debug(const Logger &logger, FileDescriptorSet &readFds, FileDescriptorSet &writeFds, bool forced)
{
	debug(logger, *this, readFds, writeFds, forced);
}

void
NIOSelector::debug(const Logger &logger, const NIOSelector &selector, FileDescriptorSet &readFds, FileDescriptorSet &writeFds, bool forced)
{
	if (!logger.isDebugEnabled())
		return;

	static std::string last;
	static unsigned long lastTime = System::currentTimeSeconds();

	int highest = selector.m_highest + 3;

	std::string line;
	line.reserve(highest + 1);

	for (int i = 0; i < highest; i++)
	{
		char c = selector.fds().test(i) ? '-' : '.';

		bool canRead = readFds.test(i);
		bool canWrite = writeFds.test(i);

		if (canRead && canWrite)
			c = 'X';
		else if (canRead)
			c = 'R';
		else if (canWrite)
			c = 'W';

		line += c;
	}

	if (forced)
		line += "  (F)";

	unsigned long now = System::currentTimeSeconds();

	if (forced || line != last || lastTime + 3 < now)
	{
		last = line;
		lastTime = now;

		logger.debug() << line << std::endl;

		logger.debug();
		for (int i = 0; i < highest; i++)
			std::cout << i;

		std::cout << std::endl;
	}
}
