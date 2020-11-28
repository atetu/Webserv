/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 19:02:22 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 19:02:22 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URL_HPP_
# define URL_HPP_

#include <util/Optional.hpp>
#include <map>
#include <string>
#include <vector>

class URL
{
	private:
		std::string m_protocol;
		std::string m_host;
		int m_port;
		std::string m_path;
		Optional<std::map<std::string, std::string> > m_queryParameters;
		Optional<std::string> m_fragment;

	public:
		URL();
		URL(const std::string &protocol, const std::string &host, int port, const std::string &path, Optional<std::map<std::string, std::string> > queryParameters, Optional<std::string> fragment);
		URL(const URL &other);

		virtual
		~URL();

		URL&
		operator =(const URL &other);

		const std::string&
		host() const;

		const std::string&
		path() const;

		int
		port() const;

		const std::string&
		protocol() const;

		const Optional<std::map<std::string, std::string> >&
		queryParameters() const;

		const Optional<std::string>&
		fragment() const;

		bool
		filename(std::string &out) const;

		bool
		extension(std::string &out) const;
};

#endif /* URL_HPP_ */
