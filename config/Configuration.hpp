/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/02 17:53:05 by alicetetu        ###   ########.fr       */
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
		typedef std::vector<ServerBlock*>::const_iterator siterator;

	private:
		std::string m_file;
		MimeRegistry *m_mimeRegistry;
	   	MimeBlock *m_mimeBlock;
		RootBlock *m_rootBlock;
		
	public:
		Configuration(void);
		Configuration(const MimeRegistry &mimeRegistry, const std::vector<ServerBlock*> &servers);
		Configuration(const std::string &file);
		Configuration(const Configuration &other);

		virtual
		~Configuration();

		Configuration&
		operator=(const Configuration &other);

		inline const MimeRegistry&
		mimeRegistry(void) const
		{
			return (*m_mimeRegistry);
		}

		const std::string& getFile(void) const;
	//	const std::vector<ServerBlock>& getServers(void) const;
		const MimeRegistry* getMimeRegistry(void) const;
		const MimeBlock* getMimeBlock(void) const;
		const RootBlock* getRootBlock(void) const;
		
		Configuration & mime(MimeBlock *mimeBlock);
		Configuration & root(RootBlock *rootBlock);
		Configuration & build();

	public:
        static Configuration 
		fromJsonFile(const std::string &path) throw (IOException);
};

#endif /* CONFIGURATION_HPP_ */
