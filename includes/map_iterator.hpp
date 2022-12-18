#ifndef MAP_ITERATOR_H
# define MAP_ITERATOR_H

# include <iostream>
# include <memory>
# include <cstddef>
# include "map.hpp"

class map;

namespace ft
{
	template <typename T, typename Node, typename Content>
	class map_iterator
	{
	public :
		typedef	std::bidirectional_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::size_t						size_type;
		typedef std::ptrdiff_t					difference_type;
		typedef Content*						pointer;
		typedef Content&						reference;

		map_iterator() : _ptr(&_present), _last(NULL), _present(), _stop() {}
		map_iterator(Node *ptr) : _ptr(ptr), _last(NULL), _present(), _stop() {}

		Node	*base() const { return _ptr; }

		value_type	&operator* () const { return *_ptr; }
		value_type	*operator-> () const { return _ptr; }

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
			return *this;
		}
		const map_iterator 	operator++ (int) { map_iterator tmp = *this; ++(*this); return tmp; }
/*		const map_iterator	&operator-- () {

		}
		const map_iterator 	operator-- (int) { map_iterator tmp = *this; --(*this); return tmp; }
*/
		const map_iterator	&operator+= (std::size_t dist) { _ptr += dist; return *this; }
		const map_iterator	&operator-= (std::size_t dist) { _ptr -= dist; return *this; }
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
		Node	*_last;
		Node	_present;
		int		_stop;
	};
}

#endif