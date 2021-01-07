/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestProcessor.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 15:59:39 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/26 15:59:39 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPROCESSOR_HPP_
# define HTTPREQUESTPROCESSOR_HPP_

class Logger;
class Configuration;
class HTTPClient;

class HTTPRequestProcessor
{
	public:
		static Logger &LOG;

	private:
		const Configuration &m_configuration;
		const Environment &m_environment;

	private:
		HTTPRequestProcessor();
		HTTPRequestProcessor(const HTTPRequestProcessor &other);

		HTTPRequestProcessor&
		operator =(const HTTPRequestProcessor &other);

	public:
		HTTPRequestProcessor(const Configuration &configuration, const Environment &environment);

		virtual
		~HTTPRequestProcessor();

		void
		process(HTTPClient &client);

		static ServerBlock const*
		findServerBlock(HTTPClient &client);
};

#endif /* HTTPREQUESTPROCESSOR_HPP_ */
