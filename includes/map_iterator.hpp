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
	template<class Node, class T>
	class map_iterator
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

		map_iterator() : _ptr(NULL), _last(NULL)  {}
		map_iterator(node_pointer ptr) : _ptr(ptr), _last(ptr) {}
		map_iterator(const map_iterator &other) : _ptr(other._ptr), _last(other._last) {}
		template<typename U>
		map_iterator(const map_iterator<Node, U> &other,
			typename ft::enable_if<!std::is_const<U>::value>::type* = 0)
				: _ptr(other._ptr), _last(other._last) {}
		map_iterator(const wrapper_it<map_iterator> &other) : _ptr(other.base()._ptr), _last(NULL) {}
		map_iterator(const reverse_iterator<map_iterator> &other) : _ptr(other.base()._ptr), _last(NULL) {}
		~map_iterator() {}

		map_iterator&	operator=(const map_iterator &assign) 
		{
			if (this != &assign)
				_ptr = assign._ptr;
			return (*this); 
		}

		pointer			base() const { return &(_ptr->data()); }
		node_pointer	node_base() const { return _ptr; }

		reference	operator* () const { return _ptr->data(); }
		pointer		operator-> () const { return &(_ptr->data()); }
		reference	operator[] (difference_type index) { return (base()[index]); }

		operator	map_iterator<Node, const value_type>() const { return (map_iterator<Node, const value_type>(_ptr)); }

		void		set_ptr(node_pointer ptr) { _ptr = ptr; }

		// pre-increment
		map_iterator& operator++() {
			if (_ptr && _ptr->_right) {
				_ptr = _ptr->_right;
				while (_ptr->_left)
					_ptr = _ptr->_left;
			} else {
				Node	*tmp = _ptr;

				_ptr = _ptr->_parent;
				while (_ptr && tmp == _ptr->_right)
				{
					tmp = _ptr;
					_ptr = _ptr->_parent;
				};
			}
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
			if (!_ptr)
				_ptr = _last;
			else if (_ptr && _ptr->_left) {
				_ptr = _ptr->_left;
				while (_ptr->_right)
					_ptr = _ptr->_right;
			} else {
				Node	*tmp = _ptr;

				_ptr = _ptr->_parent;
				while (_ptr && tmp == _ptr->_left)
				{
					tmp = _ptr;
					_ptr = _ptr->_parent;
				};
			}
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

		bool		operator== (const map_iterator &it) { return this->base() == it.base(); }

		template<typename T1, typename T2>
		friend bool	operator!= (const map_iterator<Node, T1>& lhs, const map_iterator<Node, T2>& rhs)
		{
			return (lhs._ptr != rhs._ptr);
		}

		bool		operator< (const map_iterator &it) { return this->base() < it.base(); }
		bool		operator<= (const map_iterator &it) { return this->base() <= it.base(); }
		bool		operator> (const map_iterator &it) { return this->base() > it.base(); }
		bool		operator>= (const map_iterator &it) { return this->base() >= it.base(); }

	private:
		Node	*_ptr;
		Node	*_last;
	};
}

#endif