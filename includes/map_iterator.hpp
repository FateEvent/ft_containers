#ifndef MAP_ITERATOR_H
# define MAP_ITERATOR_H

# include <iostream>
# include <memory>
# include <cstddef>
# include "map.hpp"
# include "pair.hpp"
# include "iterator_traits.hpp"

namespace ft
{
	template<class T, class U, class Category = std::bidirectional_iterator_tag,
			class Distance = ptrdiff_t, class Pointer = U*, class Reference = U&>
	class map_iterator
	{
	public :
		typedef	Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;

		map_iterator() : _ptr(NULL)  {}
		map_iterator(const map_iterator &other) : _ptr(other._ptr) {}
		map_iterator(T *ptr) : _ptr(ptr) {}
		~map_iterator() {}

		T	*get_ptr() const { return (_ptr); }

		map_iterator&	operator=(const map_iterator &assign) 
		{
			if (this != &assign)
				_ptr = assign._ptr;
			return (*this); 
		}

		reference	operator* () const { return (*_ptr); }
		pointer		operator-> () const { return (_ptr->_data); }

		// pre-increment
		map_iterator& operator++() {
			_ptr = std::next(_ptr);
			return (*this);
		}

		// post-increment
		map_iterator operator++(int) {
			map_iterator old(*this);
			operator++();
			return (old);
		}

		// pre-decrement
		map_iterator& operator--() {
			_ptr = std::prev(_ptr);
			return (*this);
		}

		// post-decrement
		map_iterator operator--(int) {
			map_iterator old(*this);
			operator--();
			return (old);
		}

		const map_iterator	operator+ (const difference_type dist) const { return (_ptr + dist); }
		const map_iterator	&operator+= (const difference_type dist) { _ptr += dist; return (*this); }
		const map_iterator	operator- (const difference_type dist) const { return (_ptr - dist); }
		const map_iterator	&operator-= (const difference_type dist) { _ptr -= dist; return (*this); }
		ptrdiff_t			operator- (const map_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const map_iterator &it) { return this->_ptr == it._ptr; }

		friend bool	operator!= (const map_iterator& lhs, const map_iterator& rhs)
		{
			return (lhs._ptr != rhs._ptr);
		}

		bool		operator< (const map_iterator &it) { return this->_ptr < it._ptr; }
		bool		operator<= (const map_iterator &it) { return this->_ptr <= it._ptr; }
		bool		operator> (const map_iterator &it) { return this->_ptr > it._ptr; }
		bool		operator>= (const map_iterator &it) { return this->_ptr >= it._ptr; }

	protected:
		pointer	_ptr;
	};

	template<class T, class U, class Iter, class Category = std::bidirectional_iterator_tag,
			class Distance = ptrdiff_t, class Pointer = U*, class Reference = U&>
	class map_const_iterator
	{
	public :
		typedef	Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;

		map_const_iterator() : _ptr(NULL)  {}
		map_const_iterator(const map_const_iterator &other) : _ptr(other._ptr) {}
		map_const_iterator(T *ptr) : _ptr(ptr) {}
		~map_const_iterator() {}

		T	*get_ptr() const { return (_ptr); }

		map_const_iterator&	operator=(const map_const_iterator &assign) 
		{
			if (this != &assign)
				_ptr = assign._ptr;
			return (*this); 
		}

		reference	operator* () const { return (_ptr->_data); }
		pointer		operator-> () const { return (&(_ptr->_data)); }
		operator	map_const_iterator<T, U, Iter>() const { return (map_const_iterator<T, U, Iter>(_ptr)); }

		// pre-increment
		map_const_iterator& operator++() {
			_ptr = _ptr->next();
			return (*this);
		}

		// post-increment
		map_const_iterator operator++(int) {
			map_const_iterator old(*this);
			operator++();
			return (old);
		}

		// pre-decrement
		map_const_iterator& operator--() {
			_ptr = _ptr->prev();
			return (*this);
		}

		// post-decrement
		map_const_iterator operator--(int) {
			map_const_iterator old(*this);
			operator--();
			return (old);
		}

		const map_const_iterator	operator+ (const difference_type dist) const { return (_ptr + dist); }
		const map_const_iterator	&operator+= (const difference_type dist) { _ptr += dist; return (*this); }
		const map_const_iterator	operator- (const difference_type dist) const { return (_ptr - dist); }
		const map_const_iterator	&operator-= (const difference_type dist) { _ptr -= dist; return (*this); }
		ptrdiff_t					operator- (const map_const_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const map_const_iterator &it) { return this->_ptr == it._ptr; }

		friend bool	operator!= (const map_const_iterator& lhs, const map_const_iterator& rhs)
		{
			return (lhs._ptr != rhs._ptr);
		}

		bool		operator< (const map_const_iterator &it) { return this->_ptr < it._ptr; }
		bool		operator<= (const map_const_iterator &it) { return this->_ptr <= it._ptr; }
		bool		operator> (const map_const_iterator &it) { return this->_ptr > it._ptr; }
		bool		operator>= (const map_const_iterator &it) { return this->_ptr >= it._ptr; }

	protected:
		pointer	_ptr;
	};
}

#endif