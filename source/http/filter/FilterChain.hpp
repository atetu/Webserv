/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilterChain.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:46:58 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 18:46:58 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILTERCHAIN_HPP_
# define FILTERCHAIN_HPP_

#include <http/filter/Filter.hpp>
#include <deque>
#include <list>

class FilterChain
{
	public:
		enum State
		{
			S_NONE = 0,
			S_BEFORE,
			S_BETWEEN,
			S_AFTER,
		};

	private:
		HTTPClient &m_client;
		Request &m_request;
		Response &m_response;

		std::deque<Filter*> m_current;

	private:
		FilterChain();
		FilterChain(const FilterChain &other);

		FilterChain&
		operator=(const FilterChain &other);

	public:
		FilterChain(HTTPClient &client, Request &request, Response &response);

		virtual
		~FilterChain();

		void
		next();

		void
		doChaining();

		inline void
		operator()()
		{
			return (next());
		}

	private:
		bool
		nextState(State state);
};

#endif /* FILTERCHAIN_HPP_ */
