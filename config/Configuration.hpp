/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/30 16:58:38 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP_
# define CONFIGURATION_HPP_

#include <config/block/CGI.hpp>
#include <config/block/LocationBlock.hpp>
#include <config/block/MimeBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <config/block/ErrorPageBlock.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <vector>

class Configuration
{
	public:
        static Logger &LOG;
	
	public:
		typedef std::vector<ServerBlock>::const_iterator siterator;

	private:
		std::vector<ServerBlock> m_servers;
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

		const std::vector<ServerBlock>&
		 servers(void) const;

		Configuration setMime();
		Configuration & root(RootBlock &rootBlock);
		Configuration & build();

	public:
        static Configuration 
		fromJsonFile(const std::string &path) throw (IOException);;
};

#endif /* CONFIGURATION_HPP_ */
