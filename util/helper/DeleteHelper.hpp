/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteHelper.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 01:47:31 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/17 01:47:31 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETEHELPER_HPP_
# define DELETEHELPER_HPP_

#include <util/Optional.hpp>
#include <list>

class DeleteHelper
{
	private:
		DeleteHelper();
		DeleteHelper(const DeleteHelper &other);

		virtual
		~DeleteHelper();

		DeleteHelper&
		operator=(const DeleteHelper &other);

	public:
		template<typename T>
			static void
			deletePointerList(Optional<std::list<T*> > &optional)
			{
				if (optional.present())
				{
					deletePointerList(optional.get());
					optional.unset();
				}
			}

		template<typename T>
			static void
			deletePointerList(Optional<std::list<T const*> > &optional)
			{
				if (optional.present())
				{
					deletePointerList(optional.get());
					optional.unset();
				}
			}

		template<typename T>
			static void
			deletePointerList(std::list<T*> &blocks)
			{
				for (typename std::list<T*>::iterator it = blocks.begin(); it != blocks.end(); it++)
					delete *it;

				blocks.clear();
			}

		template<typename T>
			static void
			deletePointerList(std::list<T const*> &blocks)
			{
				for (typename std::list<T const*>::iterator it = blocks.begin(); it != blocks.end(); it++)
					delete *it;

				blocks.clear();
			}

		template<typename T>
			static void
			deletePointer(Optional<T*> &optional)
			{
				if (optional.present())
				{
					deletePointer(optional.get());
					optional.unset();
				}
			}

		template<typename T>
			static void
			deletePointer(Optional<T const*> &optional)
			{
				if (optional.present())
				{
					deletePointer(optional.get());
					optional.unset();
				}
			}

		template<typename T>
			static void
			deletePointer(T *&ptr)
			{
				if (ptr)
					delete ptr;

				ptr = NULL;
			}

		template<typename T>
			static void
			deletePointer(T const *&ptr)
			{
				if (ptr)
					delete ptr;

				ptr = NULL;
			}
};

#endif /* DELETEHELPER_HPP_ */
