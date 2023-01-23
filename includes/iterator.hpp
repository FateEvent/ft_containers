#ifndef ITERATOR
# define ITERATOR

# include <iostream>
# include <memory>
# include <cstddef>
# include <iterator>
# include "utilities.hpp"

namespace ft
{
	template <class Cat, class T, class Dist = ptrdiff_t, class Ptr = T*, class Ref = T&>
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
		typedef T																		value_type;
		typedef std::random_access_iterator_tag											iterator_category;
		typedef typename iterator<std::random_access_iterator_tag, T>::difference_type	difference_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::reference		reference;
		typedef reference const															const_reference;
		typedef typename iterator<std::random_access_iterator_tag, T>::pointer			pointer;
		typedef pointer const															const_pointer;
//		typedef move_iterator<const T>													const_iterator;

		move_iterator() : _ptr() {}
		move_iterator(const move_iterator &it) : _ptr(it._ptr) {}
		template<typename U>
		move_iterator(const move_iterator<U> &other,
			typename ft::enable_if<!std::is_const<U>::value>::type* = 0)
				: _ptr(other._ptr) {}
		move_iterator(const pointer &ptr) : _ptr(ptr) {}
		~move_iterator() {}

		move_iterator&	operator=(const move_iterator &assign) 
		{
			if (this != &assign)
				_ptr = assign._ptr;
			return (*this); 
		}

		pointer	base() const { return (_ptr); }

		reference		operator*() { return (*_ptr); }
		const_reference	operator*() const { return (*_ptr); }
		pointer			operator->() { return &(operator*()); }
		const_pointer	operator->() const { return &(operator*()); }
		reference		operator[] (const std::size_t index) { return (base()[index]); }
//		const_reference	operator[] const (const std::size_t index) { return (base()[index]); }

		operator	move_iterator<const T>() const { return (move_iterator<const value_type>(_ptr)); }

		const move_iterator	&operator++ () { _ptr++; return *this; }
		const move_iterator	operator++ (int) { move_iterator tmp = *this; ++(*this); return tmp; }
		const move_iterator	&operator-- () { _ptr--; return *this; }
		const move_iterator	operator-- (int) { move_iterator tmp = *this; --(*this); return tmp; }		
		const move_iterator	&operator+= (const std::size_t dist) { _ptr += dist; return *this; }
		const move_iterator	&operator-= (const std::size_t dist) { _ptr -= dist; return *this; }
		const move_iterator	operator+ (const std::size_t dist) { return (_ptr + dist); }
		const move_iterator	operator- (const std::size_t dist) { return (_ptr - dist); }
		ptrdiff_t			operator- (const move_iterator &it) { return (_ptr - it._ptr); }

		friend move_iterator	operator+ (const std::size_t dist, const move_iterator &src)
		{
			move_iterator it(src._ptr);
			it += dist;
			return (it);
		}
		
		friend move_iterator	operator- (const std::size_t dist, const move_iterator &src)
		{
			move_iterator it(src._ptr);
			it -= dist;
			return (it);
		}

		friend bool	operator==(const move_iterator &lhs, const move_iterator &rhs)
		{
			return (lhs.base() == rhs.base());
		}

		friend bool	operator!=(const move_iterator &lhs, const move_iterator &rhs)
		{
			return (lhs.base() != rhs.base());
		}

		friend bool	operator>(const move_iterator &lhs, const move_iterator &rhs)
		{
			return (lhs.base() > rhs.base());
		}

		friend bool	operator<=(const move_iterator &lhs, const move_iterator &rhs)
		{
			return (lhs.base() <= rhs.base());
		}

		friend bool	operator<(const move_iterator &lhs, const move_iterator &rhs)
		{
			return (lhs.base() < rhs.base());
		}

		friend bool	operator>=(const move_iterator &lhs, const move_iterator &rhs)
		{
			return (lhs.base() >= rhs.base());
		}
/*
		operator	const_iterator(void) const
		{
			return (const_iterator(_data));
		}
*/
	protected:
		pointer		_ptr;
	};
}

#endif