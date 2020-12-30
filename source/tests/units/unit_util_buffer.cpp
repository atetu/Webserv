/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_util_string.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 00:54:37 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/26 00:54:37 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <tests/test_unit.hpp>
#include <util/buffer/impl/BaseBuffer.hpp>
#include <util/log/LoggerFactory.hpp>
#include <string>

static Logger &LOG = LoggerFactory::get("util/buffer/*");

CASE("buffer", "base-store")
{
	ASSERT(BaseBuffer().store('x').storage() == "x");
	ASSERT(BaseBuffer().store(std::string("hello")).storage() == "hello");
	ASSERT(BaseBuffer().store("hello", 5).storage() == "hello");

	return (0);
}
