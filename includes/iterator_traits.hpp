#ifndef ITERATOR_TRAITS_H
# define ITERATOR_TRAITS_H

# include <iostream>
# include <memory>
# include <cstddef>
# include <iterator>

namespace ft
{
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
		typedef Iter*							pointer;
		typedef Iter&							reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};

	template <typename Iter>
	struct iterator_traits<const Iter *>
	{
		typedef std::ptrdiff_t					difference_type;
		typedef Iter							value_type;
		typedef const Iter*						pointer;
		typedef const Iter&						reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};

	template <class Iter>
	class wrapper_it
	{
	protected:
		Iter											_current;
		typedef ft::iterator_traits<Iter>				traits_type;

	public:
		typedef Iter									value_type;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::reference			reference;
		typedef typename traits_type::pointer			pointer;
		typedef std::size_t								size_type;

		wrapper_it() : _current(Iter()) {}
		wrapper_it(const value_type &it) : _current(it) {}
		template<class U>
		wrapper_it(const wrapper_it<U>& other): _current(other.base()) {}

		wrapper_it	&operator= (wrapper_it const& other) {
			if (this == &other)
				return (*this);
			this->_current = other._current;
			return (*this);
		}

		value_type	base() const { return _current; }

		reference	operator* () { return *_current; }
		pointer		operator-> () { return &(operator*()); }

		wrapper_it	&operator++ () { _current++; return *this; }
		wrapper_it	operator++ (int) { wrapper_it tmp = *this; ++(*this); return tmp; }
		wrapper_it	&operator-- () { _current--; return *this; }
		wrapper_it	operator-- (int) { wrapper_it tmp = *this; --(*this); return tmp; }		
		wrapper_it	&operator+= (std::size_t dist) { _current += dist; return *this; }
		wrapper_it	&operator-= (std::size_t dist) { _current -= dist; return *this; }
		wrapper_it	operator+ (std::size_t dist) const { return (_current + dist); }
		wrapper_it	operator- (std::size_t dist) const { return (_current - dist); }
		reference	operator[] (std::size_t index) { return (_current[index]); }

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
		typedef Iter									value_type;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::reference			reference;
		typedef typename traits_type::pointer			pointer;
		typedef std::size_t								size_type;

		reverse_iterator() : _current(Iter()) {}
		reverse_iterator(const value_type &it) : _current(it) {}
		template<class U>
		reverse_iterator(const reverse_iterator<U>& other): _current(other.base()) {}

		reverse_iterator	&operator= (reverse_iterator const& other) {
			if (this == &other)
				return (*this);
			this->_current = other._current;
			return (*this);
		}

		value_type	base() const { return _current; }

		reference	operator* () const { return *(std::prev(_current)); }
		pointer		operator-> () { return &(operator*()); }

		reverse_iterator	&operator++ () { --_current; return std::prev(*this); }
		reverse_iterator 	operator++ (int) { reverse_iterator tmp = *this; --(*this); return tmp; }
		reverse_iterator	&operator-- () { ++_current; return *this; }
		reverse_iterator	operator-- (int) { reverse_iterator tmp = *this; ++(*this); return tmp; }
		reverse_iterator	&operator+= (std::size_t dist) { _current -= dist; return *this; }
		reverse_iterator	&operator-= (std::size_t dist) { _current += dist; return *this; }
		reverse_iterator	operator+ (std::size_t dist) { return reverse_iterator(_current - dist); }
		reverse_iterator	operator- (std::size_t dist) { return reverse_iterator(_current + dist); }
		reference			operator[] (std::size_t index) { return (std::prev(_current))[-index]; }
		ptrdiff_t			operator- (const reverse_iterator &it) { return (_current - it._current); }

		friend reverse_iterator	operator+ (const std::size_t dist, const reverse_iterator &src)
		{
			reverse_iterator it(src._current);
			it -= dist;
			return (it);
		}
		
		friend reverse_iterator	operator- (const std::size_t dist, const reverse_iterator &src)
		{
			reverse_iterator it(src._current);
			it += dist;
			return (it);
		}
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
}

#endif