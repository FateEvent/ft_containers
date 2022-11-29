#ifndef ITERATOR_H
# define ITERATOR_H

# include <iostream>
# include <memory>
# include "ArrayException.hpp"

namespace ft
{
	template <class Cat, class T, class Dist = ptrdiff_t, class Ptr = T *, class Ref = T&>
	struct iterator
	{
		typedef Cat		iterator_category;
		typedef T		value_type;
		typedef Dist	difference_type;
		typedef Ptr		pointer;
		typedef Ref		reference;
	};

	template <class T>
	class move_iterator
		: public iterator<std::random_access_iterator_tag, T>
	{
	public:
		typedef T	value_type;

		move_iterator() : _ptr() {}
		move_iterator(value_type *ptr) : _ptr(ptr) {}

		value_type	&operator* () { return *_ptr; }
		value_type	*operator-> () { return _ptr; }

		move_iterator	&operator++ () { _ptr++; return *this; }
		move_iterator	operator++ (int) { move_iterator tmp = *this; ++(*this); return tmp; }
		move_iterator	&operator-- () { _ptr--; return *this; }
		move_iterator	operator-- (int) { move_iterator tmp = *this; --(*this); return tmp; }		
		move_iterator	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		move_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		move_iterator	operator+ (std::size_t dist) const { return (_ptr + dist); }
		move_iterator	operator- (std::size_t dist) const { return (_ptr - dist); }
		move_iterator	&operator[] (std::size_t index) const { return *(*this + index); }

		bool		operator== (const move_iterator &it) { return this->_ptr == it._ptr; }
		bool		operator!= (const move_iterator &it) { return this->_ptr != it._ptr; }
		bool		operator< (const move_iterator &it) { return this->_ptr < it._ptr; }
		bool		operator<= (const move_iterator &it) { return this->_ptr <= it._ptr; }
		bool		operator> (const move_iterator &it) { return this->_ptr > it._ptr; }
		bool		operator>= (const move_iterator &it) { return this->_ptr >= it._ptr; }

	private:
		value_type	*_ptr;
	};

	template <class T>
	class const_iterator
		: public iterator<std::random_access_iterator_tag, T>
	{
		typedef T	value_type;
	
		const_iterator(value_type *ptr) : _ptr(ptr) {}

		const value_type	&operator *() const { return *_ptr; };
		const value_type	*operator ->() const { return _ptr; }

		const const_iterator&	operator ++() const { _ptr++; return *this; }
		const const_iterator	operator ++(int) const { const_iterator tmp = *this; ++(*this); return tmp; }
		const const_iterator&	operator --() const { _ptr--; return *this; }
		const const_iterator	operator --(int) const { const_iterator tmp = *this; --(*this); return tmp; }

		bool operator == (const const_iterator& it) { return this->_ptr == it._ptr; }
		bool operator != (const const_iterator& it) { return this->_ptr != it._ptr; }

	private:
		const value_type	*_ptr;
	};

	template <class T>
	struct reverse_iterator
	{
		typedef T	value_type;
		reverse_iterator(value_type *ptr) : _ptr(ptr) {}

		value_type	&operator *()	{ return *_ptr; }
		value_type	*operator ->()	{ return _ptr; }

		reverse_iterator&	operator ++() { _ptr--; return *this; }
		reverse_iterator	operator ++(int) { reverse_iterator tmp = *this; --(*this); return tmp; }
		reverse_iterator&	operator --() { _ptr++; return *this; }
		reverse_iterator	operator --(int) { reverse_iterator tmp = *this; ++(*this); return tmp; }

		bool operator ==(const reverse_iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const reverse_iterator& it) { return this->_ptr != it._ptr; }

	private:
		value_type	*_ptr;
	};

	template <class T>
	struct const_reverse_iterator
	{
		typedef T	value_type;

		const_reverse_iterator(T *ptr) : _ptr(ptr) {}

		const value_type	&operator *() const { return *_ptr; };
		const value_type	*operator ->() const { return _ptr; }

		const const_reverse_iterator&	operator ++() const { _ptr--; return *this; }
		const const_reverse_iterator	operator ++(int) const { const_reverse_iterator tmp = *this; --(*this); return tmp; }
		const const_reverse_iterator&	operator --() const { _ptr++; return *this; }
		const const_reverse_iterator	operator --(int) const { const_reverse_iterator tmp = *this; ++(*this); return tmp; }

		bool operator ==(const const_reverse_iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const const_reverse_iterator& it) { return this->_ptr != it._ptr; }

	private:
		const value_type	*_ptr;
	};

	template <class T>
	struct random_access_iterator
	{
		typedef T	value_type;

		random_access_iterator(value_type *ptr) : _ptr(ptr) {}

		value_type	&operator *()	{ return *_ptr; };
		value_type	*operator ->()	{ return _ptr; }
		value_type	&operator [](std::size_t const & index)	{ return _ptr[index]; }

		random_access_iterator&	operator ++() { _ptr++; return *this; }
		random_access_iterator	operator ++(int) { random_access_iterator tmp = *this; ++(*this); return tmp; }
		random_access_iterator&	operator --() { _ptr--; return *this; }
		random_access_iterator	operator --(int) { random_access_iterator tmp = *this; --(*this); return tmp; }

		bool operator ==(const random_access_iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const random_access_iterator& it) { return this->_ptr != it._ptr; }

	private:
		value_type	*_ptr;
	};
}

#endif