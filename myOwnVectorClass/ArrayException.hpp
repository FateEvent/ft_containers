#ifndef ARRAYEXCEPTION_H
# define ARRAYEXCEPTION_H

# include <iostream>
# include <string>
# include <exception>

class ArrayException : public std::exception
{
private:
	std::string	_msg;
public:
	explicit ArrayException(const std::string& what_arg);
	virtual ~ArrayException() throw();
	virtual const char* what() const throw();
};

#endif