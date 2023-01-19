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
//		map_iterator(const map_iterator<Key, T, Node, const value_type> &other) : _ptr(other._ptr) {}
		map_iterator(const wrapper_it<map_iterator> &other) : _ptr(other.base()._ptr) {}
		map_iterator(const reverse_iterator<map_iterator> &other) : _ptr(other.base()._ptr) {}
		~map_iterator() {}

		pointer			base() const { return &(_ptr->data()); }
		node_pointer	node_base() const { return _ptr; }

		reference	operator* () const { return _ptr->data(); }
		pointer		operator-> () const { return &(_ptr->data()); }

//		operator	map_iterator<Key, T, Node, const value_type>() const { return (map_iterator<Key, T, Node, const value_type>(_ptr)); }

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

		Node	*leftmost()
		{
			while (_ptr->left())
				_ptr = _ptr->left();
			return (_ptr);
		}

		Node	*rightmost()
		{
			while (_ptr->right())
				_ptr = _ptr->right();
			return (_ptr);
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

		const map_iterator	&operator+= (std::size_t dist) { _ptr += dist; return (*this); }
		const map_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return (*this); }
		const map_iterator	operator+ (std::size_t dist) { return (_ptr + dist); }
		const map_iterator	operator- (std::size_t dist) { return (_ptr - dist); }
		const map_iterator	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t			operator- (const map_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const map_iterator &it) { return this->base() == it.base(); }
		bool		operator!= (const map_iterator &it) { return this->base() != it.base(); }
		bool		operator< (const map_iterator &it) { return this->base() < it.base(); }
		bool		operator<= (const map_iterator &it) { return this->base() <= it.base(); }
		bool		operator> (const map_iterator &it) { return this->base() > it.base(); }
		bool		operator>= (const map_iterator &it) { return this->base() >= it.base(); }

	private:
		Node	*_ptr;
	};
}

#endif