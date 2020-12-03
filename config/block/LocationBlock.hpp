/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:10:59 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/28 19:10:59 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONBLOCK_HPP_
# define LOCATIONBLOCK_HPP_

#include <util/Optional.hpp>
#include <iostream>
#include <vector>

class LocationBlock
{
	private:
		std::string m_path;
		Optional<std::vector<std::string> > m_allowedMethods;
		Optional<std::string> m_alias;
		Optional<std::string> m_root;
		Optional<bool> m_listing;
		Optional<std::vector<std::string> > m_indexFiles;
		Optional<std::string> m_cgi;

	public:
		LocationBlock(void);
		LocationBlock(std::string path);
		LocationBlock(const LocationBlock &other);

		virtual
		~LocationBlock();

		LocationBlock&
		operator=(const LocationBlock &other);

		LocationBlock&
		methods(std::vector<std::string> methods);

		LocationBlock&
		alias(std::string alias);

		LocationBlock&
		root(std::string root);

		LocationBlock&
		listing(bool listing);

		LocationBlock&
		index(std::vector<std::string> files);

		LocationBlock&
		cgi(std::string cgi);

		const Optional<std::string>
		cgi(void) const;
		

	public:
		static LocationBlock DEFAULT;
};

#endif /* LOCATIONBLOCK_HPP_ */
