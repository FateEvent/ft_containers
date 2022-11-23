#ifndef ITERATOR_TRAITS_H
# define ITERATOR_TRAITS_H

# include <iostream>
# include <memory>
# include "ArrayException.hpp"

namespace ft
{
	template<class Cat,class T,class Dist=ptrdiff_t,class Ptr=T*,class Ref=T&> struct iterator
	{
		typedef Cat iterator_category; // §19.2.3
		typedef T value_type; // type of element
		typedef Dist difference_type; // type of iterator difference typedef Ptr pointer; // return type for – > typedef Ref reference; // return type for *
	};

	typedef struct Iterator
	{
		Iterator(int *ptr) : _ptr(ptr) {}

		int	&operator * () const { return *_ptr; }
		int	*operator -> () { return _ptr; }

		Iterator&	operator ++() { _ptr++; return *this; }
		Iterator	operator ++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		Iterator&	operator --() { _ptr--; return *this; }
		Iterator	operator --(int) { Iterator tmp = *this; --(*this); return tmp; }

		bool operator == (const Iterator& it) { return this->_ptr == it._ptr; };
		bool operator != (const Iterator& it) { return this->_ptr != it._ptr; };

	private:
		int	*_ptr;

	}				iterator;

	Iterator	begin()	{ return Iterator(&arr[0]); }
	Iterator	end()	{ return Iterator(&arr[size()]); }
}

#endif