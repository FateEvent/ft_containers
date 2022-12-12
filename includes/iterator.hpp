#ifndef ITERATOR_H
# define ITERATOR_H

# include <iostream>
# include <memory>
# include <cstddef>
# include "ContainerException.hpp"

namespace ft
{
/*
	template <typename Iter>
	struct iterator_traits
	{
		typedef typename Iter::difference_type		difference_type;
		typedef typename Iter::value_type			value_type;
		typedef typename Iter::pointer				pointer;
		typedef typename Iter::reference			reference;
		typedef typename Iter::iterator_category	iterator_category;

	};

	template <typename Iter>
	struct iterator_traits<Iter *>
	{
		typedef std::ptrdiff_t					difference_type;
		typedef Iter							value_type;
		typedef Iter *							pointer;
		typedef Iter &							reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};

	template <typename Iter>
	struct iterator_traits<const Iter *>
	{
		typedef std::ptrdiff_t					difference_type;
		typedef Iter							value_type;
		typedef const Iter *					pointer;
		typedef const Iter &					reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};

	template <class Iter>
	class wrapper_it
	{
	protected:
		Iter											_current;
		typedef iterator_traits<Iter>					traits_type;

	public:
		typedef Iter									iterator_type;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::reference			reference;
		typedef typename traits_type::pointer			pointer;
		typedef std::size_t								size_type;

		wrapper_it() : _current(Iter()) {}
		wrapper_it(const iterator_type &it) : _current(it) {}
		template<class U>
		Wrapper_it(const Wrapper_it<U>& other): current(other.base()) {}

		reference	operator* () const { return *_current; }
		pointer		operator-> () const { return &(operator*())); }

		iterator_type	base() const { return _current; }

		wrapper_it	&operator++ () { _current++; return *this; }
		wrapper_it	operator++ (int) { wrapper_it tmp = *this; ++(*this); return tmp; }
		wrapper_it	&operator-- () { _current--; return *this; }
		wrapper_it	operator-- (int) { wrapper_it tmp = *this; --(*this); return tmp; }		
		wrapper_it	&operator+= (std::size_t dist) { _current += dist; return *this; }
		wrapper_it	&operator-= (std::size_t dist) { _current -= dist; return *this; }
		wrapper_it	operator+ (std::size_t dist) const { return (_current + dist); }
		wrapper_it	operator- (std::size_t dist) const { return (_current - dist); }
		wrapper_it	&operator[] (std::size_t index) const { return *(*this + index); }
		ptrdiff_t	operator- (const wrapper_it &it) { return (_current - it._current); }
		Iter		base() const { return _current; }
	};

	template<class It1, class It2>
	bool operator== (const ft::wrapper_it<It1> &lhs, const ft::wrapper_it<It2> &rhs) { return lhs.base() == rhs.base(); }
	template<class It1, class It2>
	bool operator!= (const ft::wrapper_it<It1> &lhs, const ft::wrapper_it<It2> &rhs) { return lhs.base() != rhs.base(); }
	template<class It1, class It2>
	bool operator< (const ft::wrapper_it<It1> &lhs, const ft::wrapper_it<It2> &rhs) { return lhs.base() < rhs.base(); }
	template<class It1, class It2>
	bool operator<= (const ft::wrapper_it<It1> &lhs, const ft::wrapper_it<It2> &rhs) { return lhs.base() <= rhs.base(); }
	template<class It1, class It2>
	bool operator> (const ft::wrapper_it<It1> &lhs, const ft::wrapper_it<It2> &rhs) { return lhs.base() > rhs.base(); }
	template<class It1, class It2>
	bool operator>= (const ft::wrapper_it<It1> &lhs, const ft::wrapper_it<It2> &rhs) { return lhs.base() >= rhs.base(); }

	template <class Iter>
	class reverse_iterator
	{
	protected:
		Iter											_current;
		typedef iterator_traits<Iter>					traits_type;

	public:
		typedef Iter									iterator_type;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::reference			reference;
		typedef typename traits_type::pointer			pointer;
		typedef std::size_t								size_type;

		reverse_iterator() : _current(Iter()) {}
		reverse_iterator(const iterator_type &it) : _current(it) {}
		template<class U>
		reverse_iterator(const reverse_iterator<U>& other): current(other.base()) {}

		reference	operator* () const { return *_current; }
		pointer		operator-> () const { return &(operator*())); }

		iterator_type	base() const { return _current; }

		reverse_iterator	&operator++ () { _current--; return *this; }
		reverse_iterator 	operator++ (int) { reverse_iterator tmp = *this; --(*this); return tmp; }
		reverse_iterator	&operator-- () { _current++; return *this; }
		reverse_iterator	operator-- (int) { reverse_iterator tmp = *this; ++(*this); return tmp; }
		reverse_iterator	&operator+= (std::size_t dist) { _current += dist; return *this; }
		reverse_iterator	&operator-= (std::size_t dist) { _current -= dist; return *this; }
		reverse_iterator	operator+ (std::size_t dist) { return (_current + dist); }
		reverse_iterator	operator- (std::size_t dist) { return (_current - dist); }
		reverse_iterator	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t			operator- (const reverse_iterator &it) { return (_current - it._current); }
	};

	template<class It1, class It2>
	bool operator== (const ft::reverse_iterator<It1> &lhs, const ft::reverse_iterator<It2> &rhs) { return lhs.base() == rhs.base(); }
	template<class It1, class It2>
	bool operator!= (const ft::reverse_iterator<It1> &lhs, const ft::reverse_iterator<It2> &rhs) { return lhs.base() != rhs.base(); }
	template<class It1, class It2>
	bool operator< (const ft::reverse_iterator<It1> &lhs, const ft::reverse_iterator<It2> &rhs) { return lhs.base() < rhs.base(); }
	template<class It1, class It2>
	bool operator<= (const ft::reverse_iterator<It1> &lhs, const ft::reverse_iterator<It2> &rhs) { return lhs.base() <= rhs.base(); }
	template<class It1, class It2>
	bool operator> (const ft::reverse_iterator<It1> &lhs, const ft::reverse_iterator<It2> &rhs) { return lhs.base() > rhs.base(); }
	template<class It1, class It2>
	bool operator>= (const ft::reverse_iterator<It1> &lhs, const ft::reverse_iterator<It2> &rhs) { return lhs.base() >= rhs.base(); }
*/

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
	public:
		typedef T																		iterator_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::difference_type	difference_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::reference		reference;
		typedef typename iterator<std::random_access_iterator_tag, T>::pointer			pointer;

		move_iterator() : _ptr() {}
		move_iterator(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () { return *_ptr; }
		iterator_type	*operator-> () { return _ptr; }

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
	class const_iter
		: public iterator<std::random_access_iterator_tag, T>
	{
		public:
		typedef T	iterator_type;

		const_iter() : _ptr() {}
		const_iter(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () const { return *_ptr; }
		iterator_type	*operator-> () const { return _ptr; }

		const const_iter	&operator++ () { _ptr++; return *this; }
		const const_iter 	operator++ (int) { const_iter tmp = *this; ++(*this); return tmp; }
		const const_iter	&operator-- () { _ptr--; return *this; }
		const const_iter 	operator-- (int) { const_iter tmp = *this; --(*this); return tmp; }
		const const_iter	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		const const_iter	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		const const_iter	operator+ (std::size_t dist) { return (_ptr + dist); }
		const const_iter	operator- (std::size_t dist) { return (_ptr - dist); }
		const const_iter	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t		operator- (const const_iter &it) { return (_ptr - it._ptr); }

		bool		operator== (const const_iter &it) { return this->_ptr == it._ptr; }
		bool		operator!= (const const_iter &it) { return this->_ptr != it._ptr; }
		bool		operator< (const const_iter &it) { return this->_ptr < it._ptr; }
		bool		operator<= (const const_iter &it) { return this->_ptr <= it._ptr; }
		bool		operator> (const const_iter &it) { return this->_ptr > it._ptr; }
		bool		operator>= (const const_iter &it) { return this->_ptr >= it._ptr; }

	private:
		iterator_type	*_ptr;
	};

	template <class T>
	class rev_iter
		: public iterator<std::random_access_iterator_tag, T>
	{
		public:
		typedef T	iterator_type;

		rev_iter() : _ptr() {}
		rev_iter(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () { return *_ptr; }
		iterator_type	*operator-> () { return _ptr; }

		rev_iter	&operator++ () { _ptr--; return *this; }
		rev_iter 	operator++ (int) { rev_iter tmp = *this; --(*this); return tmp; }
		rev_iter	&operator-- () { _ptr++; return *this; }
		rev_iter	operator-- (int) { rev_iter tmp = *this; ++(*this); return tmp; }
		rev_iter	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		rev_iter	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		rev_iter	operator+ (std::size_t dist) { return (_ptr + dist); }
		rev_iter	operator- (std::size_t dist) { return (_ptr - dist); }
		rev_iter	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t			operator- (const rev_iter &it) { return (_ptr - it._ptr); }

		bool			operator== (const rev_iter &it) { return this->_ptr == it._ptr; }
		bool			operator!= (const rev_iter &it) { return this->_ptr != it._ptr; }
		bool			operator< (const rev_iter &it) { return this->_ptr < it._ptr; }
		bool			operator<= (const rev_iter &it) { return this->_ptr <= it._ptr; }
		bool			operator> (const rev_iter &it) { return this->_ptr > it._ptr; }
		bool			operator>= (const rev_iter &it) { return this->_ptr >= it._ptr; }

	private:
		iterator_type	*_ptr;
	};

	template <class T>
	class const_rev_iter
		: public iterator<std::random_access_iterator_tag, T>
	{
		public:
		typedef T	iterator_type;

		const_rev_iter() : _ptr() {}
		const_rev_iter(iterator_type *ptr) : _ptr(ptr) {}

		iterator_type	&operator* () const { return *_ptr; }
		iterator_type	*operator-> () const { return _ptr; }

		const const_rev_iter	&operator++ () { _ptr--; return *this; }
		const const_rev_iter 	operator++ (int) { const_rev_iter tmp = *this; --(*this); return tmp; }
		const const_rev_iter	&operator-- () { _ptr++; return *this; }
		const const_rev_iter	operator-- (int) { const_rev_iter tmp = *this; ++(*this); return tmp; }
		const const_rev_iter	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		const const_rev_iter	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
		const const_rev_iter	operator+ (std::size_t dist) { return (_ptr + dist); }
		const const_rev_iter	operator- (std::size_t dist) { return (_ptr - dist); }
		const const_rev_iter	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t						operator- (const const_rev_iter &it) { return (_ptr - it._ptr); }

		bool							operator== (const const_rev_iter &it) { return this->_ptr == it._ptr; }
		bool							operator!= (const const_rev_iter &it) { return this->_ptr != it._ptr; }
		bool							operator< (const const_rev_iter &it) { return this->_ptr < it._ptr; }
		bool							operator<= (const const_rev_iter &it) { return this->_ptr <= it._ptr; }
		bool							operator> (const const_rev_iter &it) { return this->_ptr > it._ptr; }
		bool							operator>= (const const_rev_iter &it) { return this->_ptr >= it._ptr; }

	private:
		iterator_type	*_ptr;
	};
}

#endif