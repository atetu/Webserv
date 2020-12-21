/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 23:36:43 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/21 23:36:43 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP_
# define FILE_HPP_

#include <stddef.h>
#include <list>
#include <string>

class File
{
	private:
		std::string m_path;

	public:
		File();
		File(std::string path);
		File(const File &other);

		virtual
		~File();

		File
		operator=(const File &other);

		bool
		exists();

		bool
		isFile();

		bool
		isDirectory();

		size_t
		length();

		std::string
		name();

		inline const std::string&
		path()
		{
			return (m_path);
		}

		std::list<File>
		list();
};

#endif /* FILE_HPP_ */
