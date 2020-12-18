/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:36:04 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/18 19:36:04 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP_
# define COMMONGATEWAYINTERFACE_HPP_

#include <util/Environment.hpp>

class CGIBlock;

class HTTPClient;

class CommonGatewayInterface
{
	private:
		const HTTPClient &m_client;
		const CGIBlock &m_cgiBlock;
		const Environment &m_environment;

	private:
		CommonGatewayInterface();
		CommonGatewayInterface(const CommonGatewayInterface &other);

		CommonGatewayInterface&
		operator=(const CommonGatewayInterface &other);

	public:
		CommonGatewayInterface(const HTTPClient &client, const CGIBlock &cgiBlock, const Environment &environment);

		virtual
		~CommonGatewayInterface();

		void
		execute();

	public:
		static const std::string ENV_AUTH_TYPE;
		static const std::string ENV_CONTENT_LENGTH;
		static const std::string ENV_CONTENT_TYPE;
		static const std::string ENV_GATEWAY_INTERFACE;
		static const std::string ENV_PATH_INFO;
		static const std::string ENV_PATH_TRANSLATED;
		static const std::string ENV_QUERY_STRING;
		static const std::string ENV_REMOTE_ADDR;
		static const std::string ENV_REMOTE_IDENT;
		static const std::string ENV_REMOTE_USER;
		static const std::string ENV_REQUEST_METHOD;
		static const std::string ENV_REQUEST_URI;
		static const std::string ENV_SCRIPT_NAME;
		static const std::string ENV_SERVER_NAME;
		static const std::string ENV_SERVER_PORT;
		static const std::string ENV_SERVER_PROTOCOL;
		static const std::string ENV_SERVER_SOFTWARE;
};

#endif /* COMMONGATEWAYINTERFACE_HPP_ */
