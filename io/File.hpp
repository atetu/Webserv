/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 23:36:43 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 18:40:48 by alicetetu        ###   ########.fr       */
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

		bool
		create(std::string location);

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

		void
		setNewPath(std::string path, std::string extension);
};

#endif /* FILE_HPP_ */
