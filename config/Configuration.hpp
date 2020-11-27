/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/27 17:53:13 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP_
# define CONFIGURATION_HPP_

#include <config/block/Block.hpp>
#include <log/Logger.hpp>
#include <log/LoggerFactory.hpp>
#include <mime/MimeRegistry.hpp>
#include <vector>

class Configuration
{
	public:
        static Logger &LOG;
	
	public:
		typedef std::vector<ServerBlock>::const_iterator siterator;

	private:
	//	std::vector<ServerBlock> m_servers;
		std::string m_file;
    	MimeRegistry m_mimeRegistry;
		//mimeRegistry.loadFromFile("mime.json");
    	RootBlock m_rootBlock;
		// {
		// 	list<CGIBlock>
		// 	list<ServerBlock>
		// }

	public:
		Configuration(void);
		Configuration(const std::string &file);
		// Configuration(std::vector<ServerBlock> servers);
		// Configuration(const Configuration &other);

		virtual
		~Configuration();

		Configuration&
		operator=(const Configuration &other);

		// const std::vector<ServerBlock>&
		// servers(void) const;

		Configuration setMime();

	public:
        static Configuration fromJsonFile(const std::string &path);
};

#endif /* CONFIGURATION_HPP_ */
