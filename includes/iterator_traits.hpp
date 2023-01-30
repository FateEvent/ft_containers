#ifndef ITERATOR_TRAITS_H
# define ITERATOR_TRAITS_H

# include <iostream>
# include <memory>
# include <cstddef>
# include <iterator>

namespace ft
{
	template<class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::difference_type	difference_type;
		typedef typename Iterator::value_type		value_type;
		typedef typename Iterator::pointer			pointer;
		typedef typename Iterator::reference		reference;
		typedef typename Iterator::iterator_category iterator_category;
	};

	template<class T>
	struct iterator_traits<T*>
	{
		typedef ptrdiff_t						difference_type;
		typedef T								value_type;
		typedef T*								pointer;
		typedef T&								reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};

	template<class T>
	struct iterator_traits<const T*>
	{
		typedef std::random_access_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::ptrdiff_t					difference_type;
		typedef const T*						pointer;
		typedef const T&						reference;
	};

	template <class Iter>
	class reverse_iterator
	{
	protected:
		Iter											_current;
		typedef ft::iterator_traits<Iter>				traits_type;

	public:
		typedef Iter									iterator_type;
		typedef typename traits_type::value_type		value_type;
		typedef typename traits_type::iterator_category	iterator_category;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::reference			reference;
		typedef typename traits_type::pointer			pointer;
		typedef std::size_t								size_type;

		reverse_iterator() : _current() {}
		reverse_iterator(iterator_type other) : _current(other) {}
		template<class U>
		reverse_iterator(const reverse_iterator<U>& other): _current(other.base()) {}
		~reverse_iterator() {}

		reverse_iterator	&operator= (reverse_iterator const& other) {
			_current = other.base();
			return (*this);
		}

		iterator_type	base() const { return _current; }

		reference	operator* () const { return ((--iterator_type(_current)).operator*()); }
		pointer		operator-> () const { return (&(operator*())); }
		reference	operator[] (difference_type index) const { return (*operator+(index)); }

		reverse_iterator	operator+ (difference_type n) const { return reverse_iterator(_current - n); }
		reverse_iterator	&operator++ () { --_current; return (*this); }
		reverse_iterator 	operator++ (int) { reverse_iterator tmp = *this; --_current; return (tmp); }
		reverse_iterator	&operator+= (difference_type n) { _current -= n; return (*this); }
		reverse_iterator	operator- (difference_type n) const { return reverse_iterator(_current + n); }
		reverse_iterator	&operator-- () { ++_current; return (*this); }
		reverse_iterator	operator-- (int) { reverse_iterator tmp = *this; ++_current; return (tmp); }
		reverse_iterator	&operator-= (difference_type n) { _current += n; return (*this); } // &?
	};

	template<class Iter1, class Iter2>
	bool	operator== (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template<class Iter1, class Iter2>
	bool	operator!= (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return (lhs.base() != rhs.base());
	}

	template<class Iter1, class Iter2>
	bool	operator< (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return (lhs.base() > rhs.base());
	}

	template<class Iter1, class Iter2>
	bool	operator<= (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return (lhs.base() >= rhs.base());
	}

	template<class Iter1, class Iter2>
	bool	operator> (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return (lhs.base() < rhs.base());
	}

	template<class Iter1, class Iter2>
	bool	operator>= (const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return (lhs.base() <= rhs.base());
	}

	template<class Iter>
	reverse_iterator<Iter>	operator+( typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter> &iter)
	{
		return (iter + n);
	}

	template<class Iter1, class Iter2>
	typename reverse_iterator<Iter1>::difference_type	operator-( const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs)
	{
		return (rhs.base() - lhs.base());
	}
}

#endif