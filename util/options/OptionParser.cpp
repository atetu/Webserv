/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 19:36:56 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/17 09:38:57 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <util/options/OptionParser.hpp>
#include <map>
#include <string>

OptionParser::OptionParser(list options) :
		m_options(options)
{
}

OptionParser::~OptionParser(void)
{
}

CommandLine
OptionParser::parse(int argc, char **argv) throw (OptionParserException)
{
	CommandLine::map values;

	for (int index = 1; index < argc; index++)
	{
		bool hasMore = (index + 1) < argc;

		char *current = argv[index];
		char *next = argv[index + 1]; /* NULL if hasMore == false. */

		if (current[0] == '-')
		{
			bool isLong = current[1] == '-';
			size_t startAt = 1 + isLong;

			std::string name = current + startAt;

			if (name.empty())
				throw OptionParserException(std::string("unparsable option: ") + current);

			bool consideredAsLong = name.length() > 1;

			const Option *found = NULL;
			for (iterator it = m_options.begin(); it != m_options.end(); it++)
			{
				const Option *option = *it;

				if (consideredAsLong && option->longName() == name)
				{
					found = option;
					break;
				}

				if (!consideredAsLong && name.at(0) == option->shortName())
				{
					found = option;
					break;
				}
			}

			if (!found)
				throw OptionParserException(std::string("unrecognized option: '") + current + std::string("'"));

			const char *argument = "";
			if (found->hasArgument())
			{
				if (!hasMore)
					throw OptionParserException(std::string("option requires an argument: '") + current + std::string(" <") + found->argumentName() + std::string(">'"));

				argument = next;
			}

			values[found->shortName()].push_back(argument);
		}
	}

	return (CommandLine(values));
}

std::string
OptionParser::help(void) const // TODO Finish
{
	std::string out;

	iterator it = m_options.begin();
	iterator ite = m_options.end();

	for (; it != ite; it++)
	{
		out += (*it)->longName() + " <" + (*it)->argumentName() + ">\n";
	}

	return (out);
}
