/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 19:31:57 by ecaceres          #+#    #+#             */
/*   Updated: 2021/01/10 19:31:57 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASK_HPP_
# define TASK_HPP_

class Task
{
	public:
		virtual
		~Task();

		virtual bool
		running() = 0;
};

#endif /* TASK_HPP_ */
