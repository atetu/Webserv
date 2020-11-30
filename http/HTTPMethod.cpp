/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:32:53 by ecaceres          #+#    #+#             */
/*   Updated: 2020/10/27 17:32:53 by ecaceres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/handler/methods/GetHandler.hpp>
#include <http/handler/methods/DeleteHandler.hpp>
#include <http/handler/methods/ConnectHandler.hpp>
#include <http/HTTPMethod.hpp>

#define DEFINE_METHOD(name, handler) ENUM_DEFINE(HTTPMethod, name, HTTPMethod(handler));

DEFINE_METHOD(GET,/*     */new GetHandler());
DEFINE_METHOD(HEAD,/*    */new GetHandler());
DEFINE_METHOD(POST,/*    */new GetHandler());
DEFINE_METHOD(PUT,/*     */new GetHandler());
DEFINE_METHOD(DELETE,/*  */new DeleteHandler());
DEFINE_METHOD(CONNECT,/* */new ConnectHandler());
DEFINE_METHOD(OPTIONS,/* */new GetHandler());
DEFINE_METHOD(TRACE,/*   */new GetHandler());

HTTPMethod::HTTPMethod(void) :
		m_handler()
{
}

HTTPMethod::HTTPMethod(HTTPMethodHandler *handler) :
		m_handler(handler)
{
}

HTTPMethod::HTTPMethod(const HTTPMethod &other) :
		m_handler(other.m_handler)
{
}

HTTPMethod&
HTTPMethod::operator =(const HTTPMethod &other)
{
	Enum::operator=(other);

	if (this != &other)
		m_handler = other.m_handler;

	return (*this);
}

HTTPMethodHandler&
HTTPMethod::handler(void) const
{
	return (*m_handler);
}
