/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIBlock.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:34:15 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/03 11:20:53 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIBLOCK_HPP_
# define CGIBLOCK_HPP_

#include <util/Optional.hpp>
#include <list>
#include <map>
#include <string>

class CGIBlock
{
	private:
		std::string m_name;
		Optional<std::string> m_path;
		Optional<bool> m_redirectErrToOut;
		Optional<std::list<std::string> > m_extensions;
		Optional<std::map<std::string, std::string> > m_environment;

	public:
		CGIBlock();
		CGIBlock(std::string name);
		CGIBlock(const CGIBlock &other);

		virtual
		~CGIBlock();

		CGIBlock&
		operator=(const CGIBlock &other);

		CGIBlock&
		path(const std::string &path);

		CGIBlock&
		redirectErrToOut(bool redirectErrToOut);

		CGIBlock&
		extensions(const std::list<std::string> &extensions);

		CGIBlock&
		environment(const std::map<std::string, std::string> &environment);

		inline const std::string&
		name(void) const
		{
			return (m_name);
		}

		inline const Optional<std::string>&
		path(void) const
		{
			return (m_path);
		}

		inline const Optional<bool>&
		redirectErrToOut(void) const
		{
			return (m_redirectErrToOut);
		}

		inline const Optional<std::list<std::string> >&
		extensions(void) const
		{
			return (m_extensions);
		}

		inline const Optional<std::map<std::string, std::string> >&
		environment(void) const
		{
			return (m_environment);
		}

		bool
		exists(void) const;

		bool
		hasExtension(const std::string &extension) const;
};

#endif /* CGIBLOCK_HPP_ */
