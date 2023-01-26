#ifndef ITERATOR_TRAITS_H
# define ITERATOR_TRAITS_H

# include <iostream>
# include <memory>
# include <cstddef>
# include <iterator>

template <class Iter>
class wrapper_it;
template <class Iter>
class reverse_iterator;

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

	template<class Category, class T, class Distance = ptrdiff_t,
			class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};

	template <class Iter>
	class wrapper_it
	{
	protected:
		Iter											_current;
		typedef ft::iterator_traits<Iter>				traits_type;

	public:
		typedef Iter									iterator_type;
		typedef typename traits_type::value_type		value_type;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::reference			reference;
		typedef typename traits_type::pointer			pointer;
		typedef std::size_t								size_type;

		wrapper_it() : _current(Iter()) {}
		wrapper_it(const iterator_type &it) : _current(it) {}
		template<class U>
		wrapper_it(const wrapper_it<U>& other): _current(other.base()) {}
		~wrapper_it() {}

		wrapper_it	&operator= (wrapper_it const& other) {
			_current = other.base();
			return (*this);
		}

		iterator_type	base() const { return _current; }

		reference	operator* () const { return (*_current); }
		pointer		operator-> () const { return &(operator*()); }

		operator	wrapper_it<const Iter>() const { return (wrapper_it<const Iter>(operator->())); }
		operator	reverse_iterator<Iter>() const { return (_current); }

		wrapper_it	&operator++ () { _current++; return *this; }
		wrapper_it	operator++ (int) { wrapper_it tmp = *this; ++(*this); return tmp; }
		wrapper_it	&operator-- () { _current--; return *this; }
		wrapper_it	operator-- (int) { wrapper_it tmp = *this; --(*this); return tmp; }		
		wrapper_it	&operator+= (size_type n) { _current += n; return *this; }
		wrapper_it	&operator-= (size_type n) { _current -= n; return *this; }
		wrapper_it	operator+ (size_type n) const { return (_current + n); }
		wrapper_it	operator- (size_type n) const { return (_current - n); }
		reference	operator[] (size_type index) { return (_current[index]); }
		ptrdiff_t	operator- (const wrapper_it &it) { return (_current - it._current); }
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
		typedef ft::iterator_traits<Iter>				traits_type;

	public:
		typedef Iter									iterator_type;
		typedef typename traits_type::value_type		value_type;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::reference			reference;
		typedef typename traits_type::pointer			pointer;
		typedef std::size_t								size_type;

		reverse_iterator() : _current() {}
		reverse_iterator(const iterator_type &it) : _current(it) {}
		template<class U>
		reverse_iterator(const reverse_iterator<U>& other): _current(other.base()) {}
		~reverse_iterator() {}

		reverse_iterator	&operator= (reverse_iterator const& other) {
			_current = other.base();
			return (*this);
		}

		template<class U>
		reverse_iterator	&operator= (reverse_iterator<U> const& other) {
			_current = other.base();
			return (*this);
		}

		iterator_type	base() const { return _current; }

		reference	operator* () const { return ((--iterator_type(_current)).operator*()); }
		pointer		operator-> () const { return (&(operator*())); }
		reference	operator[] (difference_type index) const { return (*operator+(index)); }

		operator	reverse_iterator<const Iter>() const { return (reverse_iterator<const Iter>(operator->())); }
		operator	wrapper_it<Iter>() const { return (_current); }

		reverse_iterator	operator+ (difference_type n) const { return reverse_iterator(_current - n); }
		reverse_iterator	&operator++ () { --_current; return (*this); }
		reverse_iterator 	operator++ (int) { reverse_iterator tmp = *this; --_current; return (tmp); }
		reverse_iterator	&operator+= (difference_type n) { _current -= n; return (*this); }
		reverse_iterator	operator- (difference_type n) const { return reverse_iterator(_current + n); }
		reverse_iterator	&operator-- () { ++_current; return (*this); }
		reverse_iterator	operator-- (int) { reverse_iterator tmp = *this; ++_current; return (tmp); }
		reverse_iterator	&operator-= (difference_type n) { _current += n; return (*this); }

		friend reverse_iterator	operator+ (const size_type n, const reverse_iterator &src)
		{
			reverse_iterator it(src._current);
			it -= n;
			return (it);
		}
		
		friend reverse_iterator	operator- (const size_type n, const reverse_iterator &src)
		{
			reverse_iterator it(src._current);
			it += n;
			return (it);
		}
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