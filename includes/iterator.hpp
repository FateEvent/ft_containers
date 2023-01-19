#ifndef ITERATOR
# define ITERATOR

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
		typedef T																			value_type;
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

		pointer	base() const { return _ptr; }

		const_reference	operator* () const { return *_ptr; }
		const_pointer	operator-> () const { return &(operator*()); }
		const_reference	operator[] (const std::size_t index) const { return (_ptr[index]); }
		reference		operator* () { return *_ptr; }
		pointer			operator-> () { return &(operator*()); }
		reference		operator[] (const std::size_t index) { return (_ptr[index]); }

		operator	const_iter<const T>() const { return (const_iter<const T>(operator->())); }
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