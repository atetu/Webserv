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
#include <io/FileDescriptor.hpp>
#include <tests/test_unit.hpp>
#include <util/log/LoggerFactory.hpp>
#include <string>

#ifndef O_WRONLY
#include <fcntl.h>
#endif

static Logger &LOG = LoggerFactory::get("io/File");

CASE("file", "constructor path")
{
	ASSERT(File("x").path() == "x");

	return (0);
}

CASE("file", "create and remove file")
{
	ASSERT(File("/tmp/hello").createNewFile(0666));

	ASSERT_NO_EXCEPT(File("/tmp/hello").remove());
	ASSERT_EXCEPT(File("/tmp/hello").remove());

	return (0);
}

CASE("file", "create, write, close, length, and remove")
{
	File file("/tmp/hello");

	ASSERT(file.createNewFile(0666));

	FileDescriptor *fd = NULL;
	ASSERT_NO_EXCEPT(fd = file.open(O_WRONLY));

	ASSERT(fd->write("world", 5) == 5);

	ASSERT_NO_EXCEPT(fd->close());
	ASSERT_EXCEPT(fd->close());

	ASSERT(file.length() == 5);

	ASSERT_NO_EXCEPT(File("/tmp/hello").remove());
	ASSERT_EXCEPT(File("/tmp/hello").remove());

	delete fd;

	return (0);
}
