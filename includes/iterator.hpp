#ifndef ITERATOR_H
# define ITERATOR_H

# include <iostream>
# include <memory>
# include <cstddef>
# include <iterator>

template<class T>
class move_iterator;
template<class T>
class const_iter;

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
		typedef iterator_traits<Iter>					traits_type;

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
		typedef iterator_traits<Iter>					traits_type;

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
		typedef typename iterator<std::random_access_iterator_tag, T>::difference_type	difference_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::reference		reference;
		typedef reference const															const_reference;
		typedef typename iterator<std::random_access_iterator_tag, T>::pointer			pointer;
		typedef pointer const															const_pointer;

		move_iterator() : _ptr() {}
		move_iterator(const move_iterator &it) : _ptr(it._ptr) {}
		move_iterator(const const_iter<T> &it) : _ptr(it.operator->()) {}
		move_iterator(const pointer &ptr) : _ptr(ptr) {}

		move_iterator	&operator= (move_iterator const& other) {
			if (this == &other)
				return (*this);
			this->_ptr = other._ptr;
			return (*this);
		}

		pointer	base() const { return _ptr; }

		const_reference	operator* () const { return *_ptr; }
		reference		operator* () { return *_ptr; }
		const_pointer	operator-> () const { return &(operator*()); }
		pointer			operator-> () { return &(operator*()); }
		const_reference	operator[] (const std::size_t index) const { return (_ptr[index]); }
		reference		operator[] (const std::size_t index) { return (_ptr[index]); }

		operator		const_iter<T>() const { return (_ptr); }

		move_iterator	&operator++ () { _ptr++; return *this; }
		move_iterator	operator++ (int) { move_iterator tmp = *this; ++(*this); return tmp; }
		move_iterator	&operator-- () { _ptr--; return *this; }
		move_iterator	operator-- (int) { move_iterator tmp = *this; --(*this); return tmp; }		
		move_iterator	&operator+= (const std::size_t dist) { _ptr += dist; return *this; }
		move_iterator	&operator-= (const std::size_t dist) { _ptr -= dist; return *this; }
		move_iterator	operator+ (const std::size_t dist) { return (_ptr + dist); }
		move_iterator	operator- (const std::size_t dist) { return (_ptr - dist); }
		ptrdiff_t		operator- (const move_iterator &it) { return (_ptr - it._ptr); }

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

		bool		operator== (const move_iterator &it) const { return this->base() == it.base(); }
		bool		operator!= (const move_iterator &it) const { return this->base() != it.base(); }
		bool		operator< (const move_iterator &it) const { return this->base() < it.base(); }
		bool		operator<= (const move_iterator &it) const { return this->base() <= it.base(); }
		bool		operator> (const move_iterator &it) const { return this->base() > it.base(); }
		bool		operator>= (const move_iterator &it) const { return this->base() >= it.base(); }

	protected:
		pointer		_ptr;
	};

	template<class T>
	class const_iter
		: public iterator<std::random_access_iterator_tag, T>
	{
		public:
		typedef T																		value_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::difference_type	difference_type;
		typedef typename iterator<std::random_access_iterator_tag, T>::reference		reference;
		typedef reference const															const_reference;
		typedef typename iterator<std::random_access_iterator_tag, T>::pointer			pointer;
		typedef pointer const															const_pointer;

		const_iter() : _ptr() {}
		const_iter(const const_iter &it) : _ptr(it._ptr) {}
		const_iter(const move_iterator<T> &it) : _ptr(it.operator->()) {}
		const_iter(pointer ptr) : _ptr(ptr) {}

		pointer		base() const { return _ptr; }

		const_reference	operator* () const { return *_ptr; }
		reference		operator* () { return *_ptr; }
		const_pointer	operator-> () const { return &(operator*()); }
		pointer			operator-> () { return &(operator*()); }
		const_reference	operator[] (const std::size_t index) const { return (_ptr[index]); }
		reference		operator[] (const std::size_t index) { return (_ptr[index]); }

		operator	move_iterator<T>() const { return (_ptr); }

		const_iter	&operator= (const_iter const& other) {
			if (this == &other)
				return (*this);
			this->_ptr = other._ptr;
			return (*this);
		}

		const const_iter	&operator++ () { _ptr++; return *this; }
		const const_iter 	operator++ (int) { const_iter tmp = *this; ++(*this); return tmp; }
		const const_iter	&operator-- () { _ptr--; return *this; }
		const const_iter 	operator-- (int) { const_iter tmp = *this; --(*this); return tmp; }
		const const_iter	&operator+= (const std::size_t dist) { _ptr += dist; return *this; }
		const const_iter	&operator-= (const std::size_t dist) { _ptr -= dist; return *this; }
		const const_iter	operator+ (const std::size_t dist) { return (_ptr + dist); }
		const const_iter	operator- (const std::size_t dist) { return (_ptr - dist); }
		ptrdiff_t			operator- (const const_iter &it) const { return (_ptr - it._ptr); }

		friend const_iter	operator+ (const std::size_t dist, const const_iter &src)
		{
			const_iter it(src._ptr);
			it += dist;
			return (it);
		}
		
		friend const_iter	operator- (const std::size_t dist, const const_iter &src)
		{
			const_iter it(src._ptr);
			it -= dist;
			return (it);
		}

		bool		operator== (const const_iter &it) const { return this->base() == it.base(); }
		bool		operator!= (const const_iter &it) const { return this->base() != it.base(); }
		bool		operator< (const const_iter &it) const { return this->base() < it.base(); }
		bool		operator<= (const const_iter &it) const { return this->base() <= it.base(); }
		bool		operator> (const const_iter &it) const { return this->base() > it.base(); }
		bool		operator>= (const const_iter &it) const { return this->base() >= it.base(); }

	private:
		pointer	_ptr;
	};
}

#endif