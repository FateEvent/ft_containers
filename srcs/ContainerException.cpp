#include "ContainerException.hpp"

ContainerException::ContainerException(const std::string& what_arg) : _msg(what_arg)
{

}


ContainerException::~ContainerException() throw()
{

}


const char	*ContainerException::what() const throw()
{
	return (_msg.c_str());
}
