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

#include <nio/NIOSelector.hpp>
#include <stddef.h>
#include <sys/select.h>
#include <iostream>

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

	m_operationConfig[raw] = operations;
	m_callbacks[raw] = &callback;
	m_fileDescriptors[raw] = &fd;
}

void
NIOSelector::update(FileDescriptor &fd, int operations)
{
	if (removeFromSets(fd))
		addToSets(fd, operations);
}

void
NIOSelector::remove(FileDescriptor &fd)
{
	if (removeFromSets(fd))
	{
		int raw = fd.raw();

		OperationConfigMap::iterator it1 = m_operationConfig.find(raw);
		if (it1 != m_operationConfig.end())
			m_operationConfig.erase(it1);

		CallbackMap::iterator it2 = m_callbacks.find(raw);
		if (it2 != m_callbacks.end())
			m_callbacks.erase(it2);

		FileDescriptorMap::iterator it3 = m_fileDescriptors.find(raw);
		if (it3 != m_fileDescriptors.end())
			m_fileDescriptors.erase(it3);
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
}

bool
NIOSelector::removeFromSets(FileDescriptor &fd)
{
	int opts = operations(fd);

	if (opts)
	{
		int raw = fd.raw();

		if (opts & (ACCEPT | READ))
			m_read.clear(raw);

		if (opts & (WRITE))
			m_write.clear(raw);

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
