/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 17:32:53 by ecaceres          #+#    #+#             */
/*   Updated: 2020/12/24 14:35:04 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <http/handler/methods/ConnectHandler.hpp>
#include <http/handler/methods/DeleteHandler.hpp>
#include <http/handler/methods/GetHandler.hpp>
#include <http/handler/methods/HeadHandler.hpp>
#include <http/handler/methods/PutHandler.hpp>
#include <http/HTTPMethod.hpp>

#define DEFINE_METHOD(name, handler) ENUM_DEFINE(HTTPMethod, name, HTTPMethod(&handler));

DEFINE_METHOD(GET,/*     */GetHandler::get());
DEFINE_METHOD(HEAD,/*    */HeadHandler::get());
DEFINE_METHOD(POST,/*    */GetHandler::get());
DEFINE_METHOD(PUT,/*     */PutHandler::get());
DEFINE_METHOD(DELETE,/*  */DeleteHandler::get());
DEFINE_METHOD(CONNECT,/* */ConnectHandler::get());
DEFINE_METHOD(OPTIONS,/* */GetHandler::get());
DEFINE_METHOD(TRACE,/*   */GetHandler::get());

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

HTTPMethod::~HTTPMethod(void)
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
