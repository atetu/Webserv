/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutTask.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 19:55:50 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/10 19:55:50 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUTTASK_HPP_
# define PUTTASK_HPP_

#include <http/task/HTTPTask.hpp>
#include <io/FileDescriptor.hpp>
#include <nio/NIOSelector.hpp>
#include <stddef.h>

class HTTPClient;
class HTTPRequest;

class PutTask :
		public HTTPTask,
		public NIOSelector::Callback
{
	private:
		HTTPClient &m_client;
		FileDescriptor &m_fileDescriptor;
		ssize_t m_storedCount;

	private:
		PutTask();
		PutTask(const PutTask &other);

		PutTask&
		operator=(const PutTask &other);

	public:
		PutTask(HTTPClient &client, FileDescriptor &fileDescriptor);

		virtual
		~PutTask();

		bool
		running();

		bool
		writable(FileDescriptor &fd);
};

#endif /* PUTTASK_HPP_ */
