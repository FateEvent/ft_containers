#ifndef MAP_ITERATOR_H
# define MAP_ITERATOR_H

# include <iostream>
# include <memory>
# include <cstddef>
# include "map.hpp"
# include "pair.hpp"
# include "iterator_traits.hpp"

class map;
template <class Iter>
class wrapper_it;
template <class Iter>
class reverse_iterator;

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

		reference	operator* () const { return (_ptr->value); }
		iterator	operator-> () const { return (&_ptr->value); }

		// pre-increment
		map_iterator& operator++() {
			_ptr = _ptr->next();
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
			_ptr = _ptr->prev();
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

		template<typename T1, typename T2>
		friend bool	operator!= (const map_iterator<T1, Node>& lhs, const map_iterator<T2, Node>& rhs)
		{
			return (lhs._ptr != rhs._ptr);
		}

		bool		operator< (const map_iterator &it) { return this->_ptr < it._ptr; }
		bool		operator<= (const map_iterator &it) { return this->_ptr <= it._ptr; }
		bool		operator> (const map_iterator &it) { return this->_ptr > it._ptr; }
		bool		operator>= (const map_iterator &it) { return this->_ptr >= it._ptr; }

	protected:
		T	*_ptr;
	};

	template<class T, class Node, class Iter>
	class map_const_iterator : public iterator<std::bidirectional_iterator_tag, T>
	{
	public :
		typedef	std::bidirectional_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::size_t						size_type;
		typedef std::ptrdiff_t					difference_type;
		typedef Node*							node_pointer;
		typedef Node&							node_reference;
		typedef T*								pointer;
		typedef T&								reference;

		map_const_iterator() : _ptr(NULL)  {}
		map_const_iterator(node_pointer ptr) : _ptr(ptr) {}
		map_const_iterator(const map_const_iterator &other) : _ptr(other._ptr) {}
		template<typename U>
		map_const_iterator(const map_const_iterator<U, Node, Iter> &other,
			typename ft::enable_if<!std::is_const<U>::value>::type* = 0)
				: _ptr(other._ptr){}
		map_const_iterator(const wrapper_it<map_const_iterator> &other) : _ptr(other.base()._ptr) {}
		map_const_iterator(const reverse_iterator<map_const_iterator> &other) : _ptr(other.base()._ptr) {}
		~map_const_iterator() {}

		map_const_iterator&	operator=(const map_const_iterator &assign) 
		{
			if (this != &assign)
				_ptr = assign._ptr;
			return (*this); 
		}

		pointer		base() const { return (_ptr); }

		reference	operator* () const { return (_ptr); }
		iterator	operator-> () const { return (_ptr); }
		reference	operator[] (difference_type index) { return (base()[index]); }

		operator	map_const_iterator<T, Node, Iter>() const { return (map_const_iterator<const T, Node, Iter>(_ptr)); }

		void		set_ptr(node_pointer ptr) { _ptr = ptr; }

		// pre-increment
		map_const_iterator& operator++() {
			_ptr = _ptr->next();
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

		bool	operator== (const map_const_iterator &it) { return this->base() == it.base(); }

		template<typename T1, typename T2>
		friend bool	operator!= (const map_const_iterator<T1, Node>& lhs, const map_const_iterator<T2, Node>& rhs)
		{
			return (lhs._ptr != rhs._ptr);
		}

		bool	operator< (const map_const_iterator &it) { return this->base() < it.base(); }
		bool	operator<= (const map_const_iterator &it) { return this->base() <= it.base(); }
		bool	operator> (const map_const_iterator &it) { return this->base() > it.base(); }
		bool	operator>= (const map_const_iterator &it) { return this->base() >= it.base(); }

	private:
		pointer	*_ptr;
	};
}

#endif