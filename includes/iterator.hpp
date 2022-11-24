#ifndef ITERATOR_H
# define ITERATOR_H

# include <iostream>
# include <memory>
# include "ArrayException.hpp"

namespace ft
{
/*
	template <class Cat,class T, class Dist = ptrdiff_t, class Ptr = T *, class Ref = T&>
	struct iterator
	{
		typedef Cat		iterator_category; // §19.2.3
		typedef T		value_type; // type of element
		typedef Dist	difference_type; // type of iterator difference typedef Ptr pointer; // return type for – > typedef Ref reference; // return type for *
	};
*/
	struct iterator
	{
		iterator(int *ptr) : _ptr(ptr) {}

		int	&operator *() const { return *_ptr; }
		int	*operator ->() { return _ptr; }

		iterator&	operator ++() { _ptr++; return *this; }
		iterator	operator ++(int) { iterator tmp = *this; ++(*this); return tmp; }
		iterator&	operator --() { _ptr--; return *this; }
		iterator	operator --(int) { iterator tmp = *this; --(*this); return tmp; }

		bool operator ==(const iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const iterator& it) { return this->_ptr != it._ptr; }

	private:
		int	*_ptr;
	};

	struct const_iterator
	{
		const_iterator(int *ptr) : _ptr(ptr) {}

		const int	&operator *() const { return *_ptr; };
		const int	*operator ->() { return _ptr; }

		const const_iterator&	operator ++() { _ptr++; return *this; }
		const const_iterator	operator ++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
		const const_iterator&	operator --() { _ptr--; return *this; }
		const const_iterator	operator --(int) { const_iterator tmp = *this; --(*this); return tmp; }

		bool operator ==(const const_iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const const_iterator& it) { return this->_ptr != it._ptr; }

	private:
		int	*_ptr;
	};

	struct reverse_iterator
	{
		reverse_iterator(int *ptr) : _ptr(ptr) {}

		int	&operator *() const { return *_ptr; }
		int	*operator ->() { return _ptr; }

		reverse_iterator&	operator ++() { _ptr--; return *this; }
		reverse_iterator	operator ++(int) { reverse_iterator tmp = *this; --(*this); return tmp; }
		reverse_iterator&	operator --() { _ptr++; return *this; }
		reverse_iterator	operator --(int) { reverse_iterator tmp = *this; ++(*this); return tmp; }

		bool operator ==(const reverse_iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const reverse_iterator& it) { return this->_ptr != it._ptr; }

	private:
		int	*_ptr;
	};

	struct const_reverse_iterator
	{
		const_reverse_iterator(int *ptr) : _ptr(ptr) {}

		const int	&operator *() const { return *_ptr; };
		const int	*operator ->() { return _ptr; }

		const const_reverse_iterator&	operator ++() { _ptr--; return *this; }
		const const_reverse_iterator	operator ++(int) { const_reverse_iterator tmp = *this; --(*this); return tmp; }
		const const_reverse_iterator&	operator --() { _ptr++; return *this; }
		const const_reverse_iterator	operator --(int) { const_reverse_iterator tmp = *this; ++(*this); return tmp; }

		bool operator ==(const const_reverse_iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const const_reverse_iterator& it) { return this->_ptr != it._ptr; }

	private:
		int	*_ptr;
	};


}

#endif