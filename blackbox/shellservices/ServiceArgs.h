/*
 * ServiceArgs.h
 *
 *  Created on: 13 May 2009
 *      Author: shaun
 */

#ifndef SERVICEARGS_H_
#define SERVICEARGS_H_

#define MAKE_ARG(n) (static_cast<const type##n &>(p_arg##n).m_value)
#define CHECK_ARG(n) (typeid(p_arg##n) == typeid(type##n))

namespace ShellServices
{
class ServiceArg
{
public:
	virtual ~ServiceArg() {};
};

template <typename T>
class Arg: public ServiceArg
{
public:
	Arg(T p_value):
			m_value(p_value)
	{
	}
	virtual ~Arg() {};

	mutable T m_value;
};
}

#endif /* SERVICEARGS_H_ */
