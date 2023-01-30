#ifndef ITERATOR
# define ITERATOR

# include <iostream>
# include <memory>
# include <cstddef>
# include "iterator_traits.hpp"
# include "utilities.hpp"

namespace ft
{
	template <class T>
	class move_iterator
		: public std::iterator<std::random_access_iterator_tag, T>
	{
	protected:
		typedef iterator_traits<T>				traits_type;
	public:
		typedef T								value_type;
		typedef std::random_access_iterator_tag	iterator_category;
		typedef ptrdiff_t						difference_type;
		typedef T&								reference;
		typedef const T&						const_reference;
		typedef T*								pointer;
		typedef const T*						const_pointer;
		typedef move_iterator<const T>			const_iterator;

		move_iterator() : _ptr() {}
		move_iterator(pointer ptr) : _ptr(ptr) {}
		move_iterator(const move_iterator &other) : _ptr(other._ptr) {}
		template<typename U>
		move_iterator(const move_iterator<U> &other,
			typename ft::enable_if<!std::is_const<U>::value>::type* = 0)
				: _ptr(other._ptr) {}
		~move_iterator() {}

		move_iterator&	operator=(const move_iterator &assign) 
		{
			if (this != &assign)
				_ptr = assign.base();
			return (*this); 
		}

		pointer	base() const { return (_ptr); }

		reference		operator*() { return (*_ptr); }
		const_reference	operator*() const { return (*_ptr); }
		pointer			operator->() { return &(operator*()); }
		const_pointer	operator->() const { return &(operator*()); }
		reference		operator[] (difference_type index) { return (base()[index]); }
		const_reference	operator[] (difference_type index) const { return (base()[index]); }

		move_iterator	operator+ (difference_type dist) const { return (move_iterator(base() + dist)); }
		move_iterator	&operator++ () { ++_ptr; return (*this); }
		move_iterator	operator++ (int) { move_iterator(tmp) = *this; ++_ptr; return (tmp); }
		move_iterator	&operator+= (difference_type dist) { _ptr += dist; return (*this); }
		move_iterator	operator- (difference_type dist) const { return (move_iterator(base() - dist)); }
		move_iterator	&operator-- () { --_ptr; return (*this); }
		move_iterator	operator-- (int) { move_iterator(tmp) = *this; --_ptr; return (tmp); }		
		move_iterator	&operator-= (difference_type dist) { _ptr -= dist; return (*this); }

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

		operator	const_iterator() const { return (const_iterator(_ptr)); }

	protected:
		pointer		_ptr;
	};

	template<class T>
	move_iterator<T>	operator+(typename move_iterator<T>::difference_type n, const move_iterator<T>& it)
	{
		return (it + n);
	}

	template<class Iter1, class Iter2>
	typename move_iterator<Iter1>::difference_type	operator-(const move_iterator<Iter1> &lhs, const move_iterator<Iter2> &rhs)
	{
		return (lhs.base() - rhs.base());
	}
}

#endif