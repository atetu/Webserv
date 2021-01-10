/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPTask.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 19:38:16 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/10 19:38:16 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPTASK_HPP_
# define HTTPTASK_HPP_

#include <task/Task.hpp>

class HTTPTask :
		public Task
{
	public:
		virtual
		~HTTPTask();
};

#endif /* HTTPTASK_HPP_ */
