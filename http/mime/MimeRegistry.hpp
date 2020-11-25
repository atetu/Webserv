/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeRegistry.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 17:39:30 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/26 17:39:30 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMEREGISTRY_HPP_
# define MIMEREGISTRY_HPP_

#include <http/mime/Mime.hpp>
#include <map>
#include <string>
#include <exception/IOException.hpp>
#include <util/log/Logger.hpp>

/**
 * MIME Registry to allow an easy mapping and reverse mapping between MIME string type notation (e.g. text/csv) and a corresponding file extension.
 * This class will especially be useful when returning a file as a response to a request.
 */
class MimeRegistry
{
	private:
		typedef std::map<std::string, Mime*> map;
		typedef map::const_iterator const_iterator;

	public:
		static Logger &LOG;

	private:
		map m_mapping; /* mime type -> mime */
		map m_reverse_mapping; /* file extension -> mime */

	public:
		MimeRegistry(void);
		MimeRegistry(const map &mapping, const map &reverse_mapping);
		MimeRegistry(const MimeRegistry &other);

		~MimeRegistry(void);

		MimeRegistry&
		operator =(const MimeRegistry &other);

	private:
		void
		clearMapping(map &mapping);

	public:
		/**
		 * Add a MIME to this registry.
		 * This will add it to both normal mapping and reverse mapping.
		 *
		 * @param mime MIME to add.
		 */
		void
		add(const Mime &mime);

		/**
		 * Clear all currently loaded MIME.
		 */
		void
		clear(void);

		/**
		 * Find a MIME by a MIME type. (normal mapping)
		 *
		 * @param type MIME type. (e.g. text/csv)
		 * @return Found MIME instance. (or null if none)
		 */
		const Mime*
		findByMimeType(const std::string &type) const;

		/**
		 * Find a MIME by a file extension. (reverse mapping)
		 *
		 * @param type File extension. (without the dot `.`)
		 * @return Found MIME instance. (or null if none)
		 */
		const Mime*
		findByFileExtension(const std::string &type) const;

		/**
		 * Load MIMEs from a compatible file.
		 *
		 * @param path File path.
		 * @throws IOException If the file cannot be open.
		 * @throws JsonException If the file format is not a valid JSON.
		 */
		void
		loadFromFile(const std::string &path) throw (IOException);

		/**
		 * Get the normal mapping map size.
		 *
		 * @return Normal mapping's map size.
		 */
		size_t
		size();
};

#endif /* MIMEREGISTRY_HPP_ */
