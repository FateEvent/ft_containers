#ifndef ITERATOR_H
# define ITERATOR_H

# include <iostream>
# include <memory>
# include "ArrayException.hpp"

template<
	class Category, 
	class T,
	class Distance = std::ptrdiff_t,
	class Pointer = T*,
	class Reference = T&
> struct iterator
{
	iterator(int *ptr) : _ptr(ptr) {}

	int	&operator*() const { return *_ptr; }
	int	*operator->() { return _ptr; }

	iterator&	operator ++ () { _ptr++; return *this; }  
	iterator	operator ++ (int) { iterator tmp = *this; ++(*this); return tmp; }
	iterator&	operator -- () { _ptr--; return *this; }
	iterator	operator -- (int) { iterator tmp = *this; --(*this); return tmp; }

	friend bool	operator == (const iterator& a, const iterator& b) { return a._ptr == b._ptr; };
	friend bool operator != (const iterator& a, const iterator& b) { return a._ptr != b._ptr; };   

private:
	int	*_ptr;

};

//iterator	begin()	{ return iterator(&c[0]); }
//iterator	end()	{ return iterator(&c[c.size()]); }

#endif