/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_io_file.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 23:33:53 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/25 23:33:53 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <io/File.hpp>
#include <tests/test_unit.hpp>
#include <util/log/LoggerFactory.hpp>
#include <string>

static Logger &LOG = LoggerFactory::get("io/File");

CASE("file", "constructor path")
{
	ASSERT(File("x").path() == "x");

	return (0);
}
