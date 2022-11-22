#include "ArrayException.hpp"

ArrayException::ArrayException(const std::string& what_arg) : _msg(what_arg)
{

}


ArrayException::~ArrayException() throw()
{

}


const char	*ArrayException::what() const throw()
{
	return (_msg.c_str());
}
