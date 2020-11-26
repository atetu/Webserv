/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_platform.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 23:15:51 by ecaceres          #+#    #+#             */
/*   Updated: 2020/11/25 23:15:51 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DETECT_PLATFORM_H_
# define DETECT_PLATFORM_H_

#ifdef __linux__
# define IS_LINUX 1
#elif __APPLE__
# define IS_MACOS 1
#elif __CYGWIN__
# define IS_CYGWIN 1
#else
# error Unknown plateform
#endif

#ifdef IS_LINUX or IS_MACOS
# define IS_UNIX 1
#endif

#ifdef IS_CYGWIN
# define IS_WINDOWS 1
#endif

#endif /* DETECT_PLATFORM_H_ */
