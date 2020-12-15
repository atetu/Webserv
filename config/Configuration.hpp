/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:33:47 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/09 11:25:39 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP_
# define CONFIGURATION_HPP_

#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <http/mime/MimeRegistry.hpp>
#include <string>
#include <vector>

// TODO: All blocks need deep-copy, same goes for mime registry
// Configuration should not be passed around like a normal object, but must be kept as singleton if possible

class Configuration
{
	public:
		static Logger &LOG;

	private:
		std::string m_file;
		MimeRegistry *m_mimeRegistry;
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

		inline const std::string&
		file(void) const
		{
			return (m_file);
		}

		inline const MimeRegistry&
		mimeRegistry(void) const
		{
			return (*m_mimeRegistry);
		}

		inline const RootBlock&
		rootBlock(void) const
		{
			return (*m_rootBlock);
		}

		Configuration&
		build();

	public:
		static Configuration
		fromJsonFile(const std::string &path) throw (IOException);
};

#endif /* CONFIGURATION_HPP_ */
