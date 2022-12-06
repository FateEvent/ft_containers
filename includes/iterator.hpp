#ifndef ITERATOR_H
# define ITERATOR_H

# include <iostream>
# include <memory>
# include "ContainerException.hpp"
#include <iterator>	// implement iterator_traits

namespace ft
{
	template <class Cat, class T, class Dist = ptrdiff_t, class Ptr = T *, class Ref = T&>
	struct iterator
	{
		typedef Cat		iterator_category;
		typedef T		iterator_type;
		typedef Dist	difference_type;
		typedef Ptr		pointer;
		typedef Ref		reference;
	};

	template <class T>
	class move_iterator
		: public iterator<std::random_access_iterator_tag, T>
	{
	protected:
		T	_current;
	public:
		typedef T																		iterator_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::difference_type	difference_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::reference		reference;
		typedef typename iterator<std::random_access_iterator_tag, T>::pointer			pointer;

		move_iterator() : _ptr() {}
		move_iterator(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () { return *_ptr; }
		iterator_type	*operator-> () { return _ptr; }

		iterator_type	base() const { return _current; }

		move_iterator	&operator++ () { _ptr++; return *this; }
		move_iterator	operator++ (int) { move_iterator tmp = *this; ++(*this); return tmp; }
		move_iterator	&operator-- () { _ptr--; return *this; }
		move_iterator	operator-- (int) { move_iterator tmp = *this; --(*this); return tmp; }		
		move_iterator	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		move_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		move_iterator	operator+ (std::size_t dist) const { return (_ptr + dist); }
		move_iterator	operator- (std::size_t dist) const { return (_ptr - dist); }
		move_iterator	&operator[] (std::size_t index) const { return *(*this + index); }
		ptrdiff_t		operator- (const move_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const move_iterator &it) { return this->_ptr == it._ptr; }
		bool		operator!= (const move_iterator &it) { return this->_ptr != it._ptr; }
		bool		operator< (const move_iterator &it) { return this->_ptr < it._ptr; }
		bool		operator<= (const move_iterator &it) { return this->_ptr <= it._ptr; }
		bool		operator> (const move_iterator &it) { return this->_ptr > it._ptr; }
		bool		operator>= (const move_iterator &it) { return this->_ptr >= it._ptr; }

	protected:
		pointer	_ptr;
	};

	template <class T>
	class const_iterator
		: public iterator<std::random_access_iterator_tag, T>
	{
		public:
		typedef T	iterator_type;

		const_iterator() : _ptr() {}
		const_iterator(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () const { return *_ptr; }
		iterator_type	*operator-> () const { return _ptr; }

		const const_iterator	&operator++ () { _ptr++; return *this; }
		const const_iterator 	operator++ (int) { const_iterator tmp = *this; ++(*this); return tmp; }
		const const_iterator	&operator-- () { _ptr--; return *this; }
		const const_iterator 	operator-- (int) { const_iterator tmp = *this; --(*this); return tmp; }
		const const_iterator	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		const const_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		const const_iterator	operator+ (std::size_t dist) { return (_ptr + dist); }
		const const_iterator	operator- (std::size_t dist) { return (_ptr - dist); }
		const const_iterator	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t		operator- (const const_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const const_iterator &it) { return this->_ptr == it._ptr; }
		bool		operator!= (const const_iterator &it) { return this->_ptr != it._ptr; }
		bool		operator< (const const_iterator &it) { return this->_ptr < it._ptr; }
		bool		operator<= (const const_iterator &it) { return this->_ptr <= it._ptr; }
		bool		operator> (const const_iterator &it) { return this->_ptr > it._ptr; }
		bool		operator>= (const const_iterator &it) { return this->_ptr >= it._ptr; }

	private:
		iterator_type	*_ptr;
	};

	template <class T>
	class reverse_iterator
		: public iterator<std::random_access_iterator_tag, T>
	{
		public:
		typedef T	iterator_type;

		reverse_iterator() : _ptr() {}
		reverse_iterator(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () { return *_ptr; }
		iterator_type	*operator-> () { return _ptr; }

		reverse_iterator	&operator++ () { _ptr--; return *this; }
		reverse_iterator 	operator++ (int) { reverse_iterator tmp = *this; --(*this); return tmp; }
		reverse_iterator	&operator-- () { _ptr++; return *this; }
		reverse_iterator	operator-- (int) { reverse_iterator tmp = *this; ++(*this); return tmp; }
		reverse_iterator	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		reverse_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		reverse_iterator	operator+ (std::size_t dist) { return (_ptr + dist); }
		reverse_iterator	operator- (std::size_t dist) { return (_ptr - dist); }
		reverse_iterator	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t			operator- (const reverse_iterator &it) { return (_ptr - it._ptr); }

		bool			operator== (const reverse_iterator &it) { return this->_ptr == it._ptr; }
		bool			operator!= (const reverse_iterator &it) { return this->_ptr != it._ptr; }
		bool			operator< (const reverse_iterator &it) { return this->_ptr < it._ptr; }
		bool			operator<= (const reverse_iterator &it) { return this->_ptr <= it._ptr; }
		bool			operator> (const reverse_iterator &it) { return this->_ptr > it._ptr; }
		bool			operator>= (const reverse_iterator &it) { return this->_ptr >= it._ptr; }

	private:
		iterator_type	*_ptr;
	};

	template <class T>
	class const_reverse_iterator
		: public iterator<std::random_access_iterator_tag, T>
	{
		public:
		typedef T	iterator_type;

		const_reverse_iterator() : _ptr() {}
		const_reverse_iterator(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () const { return *_ptr; }
		iterator_type	*operator-> () const { return _ptr; }

		const const_reverse_iterator	&operator++ () { _ptr--; return *this; }
		const const_reverse_iterator 	operator++ (int) { const_reverse_iterator tmp = *this; --(*this); return tmp; }
		const const_reverse_iterator	&operator-- () { _ptr++; return *this; }
		const const_reverse_iterator	operator-- (int) { const_reverse_iterator tmp = *this; ++(*this); return tmp; }
		const const_reverse_iterator	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		const const_reverse_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		const const_reverse_iterator	operator+ (std::size_t dist) { return (_ptr + dist); }
		const const_reverse_iterator	operator- (std::size_t dist) { return (_ptr - dist); }
		const const_reverse_iterator	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t						operator- (const const_reverse_iterator &it) { return (_ptr - it._ptr); }

		bool							operator== (const const_reverse_iterator &it) { return this->_ptr == it._ptr; }
		bool							operator!= (const const_reverse_iterator &it) { return this->_ptr != it._ptr; }
		bool							operator< (const const_reverse_iterator &it) { return this->_ptr < it._ptr; }
		bool							operator<= (const const_reverse_iterator &it) { return this->_ptr <= it._ptr; }
		bool							operator> (const const_reverse_iterator &it) { return this->_ptr > it._ptr; }
		bool							operator>= (const const_reverse_iterator &it) { return this->_ptr >= it._ptr; }

	private:
		iterator_type	*_ptr;
	};

	template <class T>
	struct random_access_iterator
	{
		typedef T	iterator_type;

		random_access_iterator(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator *()	{ return *_ptr; };
		iterator_type	*operator ->()	{ return _ptr; }
		iterator_type	&operator [](std::size_t const & index)	{ return _ptr[index]; }

		random_access_iterator&	operator ++() { _ptr++; return *this; }
		random_access_iterator	operator ++(int) { random_access_iterator tmp = *this; ++(*this); return tmp; }
		random_access_iterator&	operator --() { _ptr--; return *this; }
		random_access_iterator	operator --(int) { random_access_iterator tmp = *this; --(*this); return tmp; }

		bool operator ==(const random_access_iterator& it) { return this->_ptr == it._ptr; }
		bool operator !=(const random_access_iterator& it) { return this->_ptr != it._ptr; }

	private:
		iterator_type	*_ptr;
	};
}

#endif