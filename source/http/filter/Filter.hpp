/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Filter.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:45:19 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/06 18:45:19 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILTER_HPP_
# define FILTER_HPP_

class HTTPClient;
class Request;
class Response;
class FilterChain;

class Filter
{
	public:
		virtual
		~Filter();

		virtual void
		doFilter(HTTPClient &client, Request &request, Response &response, FilterChain &next) = 0;
};

#endif /* FILTER_HPP_ */
