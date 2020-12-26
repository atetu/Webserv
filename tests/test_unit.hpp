/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_unit.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 23:18:42 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/25 23:18:42 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_UNIT_HPP_
# define TEST_UNIT_HPP_

# ifndef RUN_TESTS
#  define RUN_TESTS 0
# endif

#include <tests/case/TestCase.hpp>
#include <list>
#include <map>
#include <string>

# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)

/* from: https://stackoverflow.com/a/1597129/7292958 */
# define TOKENPASTE(x, y) x ## y
# define TOKENPASTE2(x, y) TOKENPASTE(x, y)

# define CASE_OBJECT_NAME TOKENPASTE2(AUTO_DECLARED_CASE, __LINE__)
# define CASE_METHOD_NAME TOKENPASTE2(AUTO_DECLARED_METHOD, __LINE__)

# define FILE_LOCATION __FILE__ ":" TOSTRING(__LINE__)

# define CASE(unit, name) \
	static int CASE_METHOD_NAME(int argc, char **argv, char **envp); \
	static TestCase CASE_OBJECT_NAME(unit, name, FILE_LOCATION, &CASE_METHOD_NAME); \
	static int CASE_METHOD_NAME(int argc, char **argv, char **envp)

# define ASSERT(cond) \
	if (!(cond)) \
	{ \
		LOG.fatal() << __FILE__ << ":" << __LINE__ << " -- " << #cond << std::endl; \
		return (1); \
	}

extern std::map<std::string, std::list<TestCase> > g_test_units_and_cases;

int
test_main(int argc, char **argv, char **envp);

#endif /* TEST_UNIT_HPP_ */
