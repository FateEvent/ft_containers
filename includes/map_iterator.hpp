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
	template <class Key, class T, class Node, class Content>
	class map_iterator
	{
	public :
		typedef	std::bidirectional_iterator_tag		iterator_category;
		typedef Key									key_type;
		typedef T									mapped_type;
		typedef Content								value_type;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef Node*								node_pointer;
		typedef Node&								node_reference;
		typedef Content*							pointer;
		typedef Content&							reference;

		map_iterator() : _ptr(NULL) {}
		map_iterator(node_pointer ptr) : _ptr(ptr) {}
		map_iterator(const map_iterator &other) : _ptr(other._ptr) {}
		template<typename U>
		map_iterator(const map_iterator<Key, T, Node, U> &other,
			typename ft::enable_if<!std::is_const<U>::value>::type* = 0)
				: _ptr(other._ptr) {}
		map_iterator(const wrapper_it<map_iterator> &other) : _ptr(other.base()._ptr) {}
		map_iterator(const reverse_iterator<map_iterator> &other) : _ptr(other.base()._ptr) {}
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

		operator	map_iterator<Key, T, Node, const value_type>() const { return (map_iterator<Key, T, Node, const value_type>(_ptr)); }

		void		set_ptr(node_pointer ptr) { _ptr = ptr; }

		static Node	*local_Rb_tree_decrement(Node *_x) throw ()
		{
			if (_x->_left != 0)
			{
				_x = _x->_left;
				while (_x->_right != 0)
				_x = _x->_right;
			}
			else
			{
				Node *_y = _x->_parent;
				while (_x == _y->_left)
				{
					_x = _y;
					_y = _y->_parent;
				}
				if (_x->_left != _y)
				_x = _y;
			}
			return _x;
		}

		Node *_Rb_tree_decrement(Node *_x) throw ()
		{
			return local_Rb_tree_decrement(_x);
		}

		const Node *_Rb_tree_decrement(const Node *_x) throw ()
		{
			return local_Rb_tree_decrement(const_cast<Node *>(_x));
		}

		static Node	*local_Rb_tree_increment(Node *_x) throw ()
		{
			if (_x->_right != 0)
			{
				_x = _x->_right;
				while (_x->_left != 0)
				_x = _x->_left;
			}
			else
			{
				Node *_y = _x->_parent;
				while (_x == _y->_right)
				{
					_x = _y;
					_y = _y->_parent;
				}
				if (_x->_right != _y)
				_x = _y;
			}
			return _x;
		}

		Node *_Rb_tree_increment(Node *_x) throw ()
		{
			return local_Rb_tree_increment(_x);
		}

		const Node *_Rb_tree_increment(const Node *_x) throw ()
		{
			return local_Rb_tree_increment(const_cast<Node *>(_x));
		}

		const map_iterator	&operator++ () {
			_ptr = _Rb_tree_increment(_ptr);
			return (*this);
		}
		const map_iterator 	operator++ (int) { map_iterator tmp = *this; ++(*this); return tmp; }
		const map_iterator	&operator-- () {
			_ptr = _Rb_tree_decrement(_ptr);
			return (*this);
		}
		const map_iterator 	operator-- (int) { map_iterator tmp = *this; --(*this); return tmp; }

		const map_iterator	operator+ (const difference_type dist) const { return (_ptr + dist); }
		const map_iterator	&operator+= (const difference_type dist) { _ptr += dist; return (*this); }
		const map_iterator	operator- (const difference_type dist) const { return (_ptr - dist); }
		const map_iterator	&operator-= (const difference_type dist) { _ptr -= dist; return (*this); }
		ptrdiff_t			operator- (const map_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const map_iterator &it) { return this->base() == it.base(); }

		template<typename T1, typename T2>
		friend bool	operator!= (const map_iterator<Key, T, Node, T1>& lhs, const map_iterator<Key, T, Node, T2>& rhs)
		{
			return (lhs._ptr != rhs._ptr);
		}

		bool		operator< (const map_iterator &it) { return this->base() < it.base(); }
		bool		operator<= (const map_iterator &it) { return this->base() <= it.base(); }
		bool		operator> (const map_iterator &it) { return this->base() > it.base(); }
		bool		operator>= (const map_iterator &it) { return this->base() >= it.base(); }

	private:
		Node	*_ptr;
	};
}

#endif