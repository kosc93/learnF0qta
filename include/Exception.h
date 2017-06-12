/*
 * Exceptions.h
 *
 *  Created on: 15 May 2017
 *      Author: patrick
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <iostream>
#include <exception>

class Exception : public std::exception {
public:
	Exception(const std::string& msg): m_message(msg) {}

	const char * what () const throw ()
	{
		return m_message.c_str();
	}

protected:
	std::string m_message;
};



#endif /* EXCEPTION_H_ */
