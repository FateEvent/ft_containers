#ifndef CONTAINEREXCEPTION_H
# define CONTAINEREXCEPTION_H

# include <iostream>
# include <string>
# include <exception>

class ContainerException : public std::exception
{
private:
	std::string	_msg;
public:
	explicit ContainerException(const std::string& what_arg) : _msg(what_arg) {}
	virtual ~ContainerException() throw() {}
	virtual const char* what() const throw() { return (_msg.c_str()); }
};

#endif