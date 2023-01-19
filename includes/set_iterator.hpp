#ifndef SET_ITERATOR_H
# define SET_ITERATOR_H

# include <iostream>
# include <memory>
# include <cstddef>
# include "set.hpp"
# include "pair.hpp"

class set;

namespace ft
{
	template <class Key, class T, class Node, class Content>
	class set_iterator
	{
	public :
		typedef	std::bidirectional_iterator_tag		iterator_category;
		typedef Key									key_type;
		typedef T									setped_type;
		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef Node*								node_pointer;
		typedef Node&								node_reference;
		typedef Content*							pointer;
		typedef Content&							reference;

		set_iterator() : _ptr(NULL) {}
		set_iterator(node_pointer ptr) : _ptr(ptr) {}
		set_iterator(const set_iterator &other) : _ptr(other._ptr) {}

		pointer			base() const { return &(_ptr->data()); }
		node_pointer	node_base() const { return _ptr; }

		reference	operator* () const { return _ptr->data(); }	// regarder la doc su les itérateurs
		pointer		operator-> () const { return &(_ptr->data()); }	// OK!

		void			set_ptr(node_pointer ptr) { _ptr = ptr; }

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

		const set_iterator	&operator++ () {
			_ptr = _Rb_tree_increment(_ptr);
			return (*this);
		}
		const set_iterator 	operator++ (int) { set_iterator tmp = *this; ++(*this); return tmp; }
		const set_iterator	&operator-- () {
			_ptr = _Rb_tree_decrement(_ptr);
			return (*this);
		}
		const set_iterator 	operator-- (int) { set_iterator tmp = *this; --(*this); return tmp; }

		const set_iterator	&operator+= (std::size_t dist) { _ptr += dist; return (*this); }
		const set_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return (*this); }
		const set_iterator	operator+ (std::size_t dist) { return (_ptr + dist); }
		const set_iterator	operator- (std::size_t dist) { return (_ptr - dist); }
		const set_iterator	&operator[] (std::size_t index) { return *(*this + index); }
		ptrdiff_t			operator- (const set_iterator &it) { return (_ptr - it._ptr); }

		bool		operator== (const set_iterator &it) { return this->base() == it.base(); }
		bool		operator!= (const set_iterator &it) { return this->base() != it.base(); }
		bool		operator< (const set_iterator &it) { return this->base() < it.base(); }
		bool		operator<= (const set_iterator &it) { return this->base() <= it.base(); }
		bool		operator> (const set_iterator &it) { return this->base() > it.base(); }
		bool		operator>= (const set_iterator &it) { return this->base() >= it.base(); }

	private:
		Node	*_ptr;
	};
}

#endif